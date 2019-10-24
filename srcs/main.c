#include "woody.h"
#include <stdio.h>

void    get_payload(struct s_woody* wrapper)
{

}

int		inject_payload(struct s_woody* wrapper)
{
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

	struct s_woody_file new_file;
	wrapper.new_file = &new_file;
	if (create_file(&new_file, &file_to_pack))
	{
		perror(av[0]);
		return (1);
	}	
	inject_payload(&wrapper);
	build_header_elf64(&wrapper);
	debug_print_file(&new_file);
	close_file(&file_to_pack);
	close_file(&new_file);
}