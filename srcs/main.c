#include "woody.h"
#include <stdio.h>

int    get_payload(struct s_woody* wrapper)
{
	if (fill_file(DEFAULT_PAYLOAD, wrapper->file_to_inject))
	{
		perror(wrapper->prog_name);
		return (1);
	}
	Elf64_Shdr* p_txt_sec_ptr = find_section(wrapper->file_to_inject->ptr, ".text");
	wrapper->payload.size = p_txt_sec_ptr->sh_size;
	wrapper->payload.data = wrapper->file_to_inject->ptr + p_txt_sec_ptr->sh_offset;
	int i = wrapper->payload.size;
	int count = 0;
	while (i > 0 && count != 8)
	{
		printf("%lx\n", wrapper->payload.data[i]);
		if (wrapper->payload.data[i] == 0x11)
		{
			count++;
		}
		else
		{
			count = 0;
		}
		if (count == 8)
		{
			uint64_t *tmp;
			tmp = (uint64_t *)(wrapper->payload.data + i);
			*tmp = (uint64_t)((uint64_t)(wrapper->old_entry));
			printf("TMP : %lx\n", *tmp);
		}
		i -= 1;
	}
	 i = wrapper->payload.size;
	 count = 0;
	while (i > 0 && count != 8)
	{
		printf("%lx\n", wrapper->payload.data[i]);
		if (wrapper->payload.data[i] == 0x22)
		{
			count++;
		}
		else
		{
			count = 0;
		}
		if (count == 8)
		{
			uint64_t *tmp;
			tmp = (uint64_t *)(wrapper->payload.data + i);
			*tmp = (uint64_t)((uint64_t)(wrapper->new_entry));
			printf("TMP : %lx\n", *tmp);
		}
		i -= 1;
	}
	return (0);
}

int		inject_payload(struct s_woody* wrapper)
{
	int offset;
	int available_size;
	Elf64_Phdr* t_txt_seg_ptr = find_codecave(wrapper->file_to_pack->ptr, wrapper->file_to_pack->st.st_size, &offset, &available_size);
	uintptr_t base = t_txt_seg_ptr->p_vaddr;

	if(wrapper->payload.size > available_size)
	{
		perror("[!] Payload to big to inject.\n");
		return (1);
	}
	printf("BASE: %lx\n", base);
	printf("TEXT_END: %lx\n", offset);
	printf("AVAIL: %ld\n", available_size);
	wrapper->file_to_pack->header_union.elf64->e_entry = (Elf64_Addr)(offset);
	wrapper->new_entry = (uintptr_t)offset;
	get_payload(wrapper);
	memmove(wrapper->file_to_pack->ptr + offset, wrapper->payload.data, wrapper->payload.size);

	if (write(wrapper->new_file->fd, wrapper->file_to_pack->ptr, wrapper->file_to_pack->st.st_size) == 0)
		perror("MERDE");

	return (0);
}

int main(int ac, char** av)
{
	printf("ADDR %p\n", main);
    struct s_woody wrapper;

    if (ac != 2)
    {
        ft_printf("Usage: %s file\n", av[0]);
        return (0);
    }
    wrapper.prog_name = av[0];

	struct s_woody_file file_to_pack;
	wrapper.file_to_pack = &file_to_pack;
	if (fill_file(av[1], &file_to_pack))
	{
		perror(av[0]);
		return (1);
	}
	debug_print_file(&file_to_pack);

	if (file_to_pack.type != E_TYPE_ELF64)
	{
		ft_dprintf(2, "%s: only 64 bit ELF is supported\n", av[0]);
		close_file(&file_to_pack);
		return (1);
	}

	wrapper.old_entry = (void*)file_to_pack.header_union.elf64->e_entry;
	struct s_woody_file new_file;
	wrapper.new_file = &new_file;
	if (create_file(&new_file, &file_to_pack))
	{
		perror(av[0]);
		return (1);
	}

	struct s_woody_file file_to_inject;
	wrapper.file_to_inject = &file_to_inject;

	inject_payload(&wrapper);
	build_header_elf64(&wrapper);
	debug_print_file(&new_file);

	Elf64_Shdr* p_txt_sec_ptr = find_section(file_to_pack.ptr, ".text");

	close_file(&file_to_pack);
	close_file(&new_file);
}