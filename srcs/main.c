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
	wrapper->payload.data = (char*)p_txt_sec_ptr;
	return (0);
}

int		inject_payload(struct s_woody* wrapper)
{
	wrapper->file_to_pack->header_union.elf64->e_entry = wrapper->old_entry + wrapper->file_to_pack->st.st_size;
    printf("ENTRY %p\n", wrapper->file_to_pack->header_union.elf64->e_entry);
	if (write(wrapper->new_file->fd, wrapper->file_to_pack->ptr, wrapper->file_to_pack->st.st_size) == 0)
		perror("MERDE");
	write(wrapper->new_file->fd, wrapper->file_to_inject->ptr, wrapper->file_to_inject->st.st_size);
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
	get_payload(&wrapper);

	inject_payload(&wrapper);
	build_header_elf64(&wrapper);
	debug_print_file(&new_file);

	Elf64_Shdr* p_txt_sec_ptr = find_section(file_to_pack.ptr, ".text");

	close_file(&file_to_pack);
	close_file(&new_file);
}