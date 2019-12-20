#include "woody.h"
#include <stdio.h>
#include <time.h>

void	replace_pattern(uint64_t pattern, char* mem, size_t size, uint64_t replace)
{
	int i = 0;
	while (i < size)
	{
		uint64_t *tmp;
		tmp = (uint64_t *)(mem + i);
		if (*tmp == pattern)
		{
			*tmp = replace;
		}
		i += 1;
	}
}

void	test_rot(char* ptr, size_t size)
{
	int i = 0;

	while (i < size)
	{
		ptr[i] = ptr[i] + 42;
		i++;
	}
}

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
	replace_pattern(0x1111111111111111L, wrapper->payload.data, wrapper->payload.size, wrapper->old_entry - wrapper->new_entry);	
}

int		inject_payload(struct s_woody* wrapper)
{
	int offset;
	int available_size;
	Elf64_Phdr* t_txt_seg_ptr = find_codecave(wrapper->file_to_pack->ptr, wrapper->file_to_pack->st.st_size, &offset, &available_size);
	if ((void*)t_txt_seg_ptr < wrapper->file_to_pack->ptr)
		return (1);
	
	printf("%x\n", t_txt_seg_ptr->p_filesz);
	
	rc4_crypt(wrapper->file_to_pack->ptr + t_txt_seg_ptr->p_offset, t_txt_seg_ptr->p_filesz, wrapper->key);
	// rc4_crypt(wrapper->file_to_pack->ptr + t_txt_seg_ptr->p_offset, t_txt_seg_ptr->p_filesz, wrapper->key);
	wrapper->file_to_pack->header_union.elf64->e_entry = (Elf64_Addr)(offset);
	wrapper->new_entry = (void*)(uint64_t)offset;
	get_payload(wrapper);

	if(wrapper->payload.size > available_size)
	{
		printf("[!] Payload too big to inject.\n");
		return (1);
	}
	replace_pattern(0x2222222222222222L, wrapper->payload.data, wrapper->payload.size, (uint64_t)(t_txt_seg_ptr->p_offset - (uint64_t)wrapper->new_entry));	
	replace_pattern(0x3333333333333333L, wrapper->payload.data, wrapper->payload.size, (uint64_t)(t_txt_seg_ptr->p_filesz));	
	replace_pattern(0x4444444444444444L, wrapper->payload.data, wrapper->payload.size, *(uint64_t*)(wrapper->key));	

	t_txt_seg_ptr->p_flags |= PF_W;


	memmove(wrapper->file_to_pack->ptr + offset, wrapper->payload.data, wrapper->payload.size);
	if (write(wrapper->new_file->fd, wrapper->file_to_pack->ptr, wrapper->file_to_pack->st.st_size) == 0)
		perror("MERDE");

	return (0);
}

int main(int ac, char** av)
{
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

	srand(time(NULL));
	uint64_t k = generate_key();

	wrapper.key = (uint8_t *)&k;

	if (inject_payload(&wrapper))
	{
		printf("%s: fatal: could not inject payload.\n", av[0]);
		goto end;
	}
	//build_header_elf64(&wrapper);
	//debug_print_file(&new_file);




	printf("Key: 0x%016llx\n", *(uint64_t*)(wrapper.key));

end:
	free(new_file.path);
	close_file(&file_to_pack);
	close_file(&new_file);
}