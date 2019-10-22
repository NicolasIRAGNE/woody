#include "woody.h"
#include <stdio.h>

void    get_payload(struct s_woody* wrapper)
{

}

int		inject_payload(struct s_woody* wrapper)
{
    int new_fd;

    char* new_file;
    new_file = malloc(sizeof(PACKED_PREFIX) + sizeof(PACKED_SUFFIX) + strlen(wrapper->file_to_pack->path));
    sprintf(new_file, "%s%s%s", PACKED_PREFIX, wrapper->file_to_pack->path, PACKED_SUFFIX);
    new_fd = open(new_file, O_WRONLY | O_CREAT | O_TRUNC, wrapper->file_to_pack->st.st_mode & RWX_UGO);
    if (new_fd < 0)
    {
        perror(wrapper->prog_name);
        return (1);
    }
    close(new_fd);
	return (0);
}

int main(int ac, char** av)
{
    struct s_woody wrapper;

    if (ac != 2)
    {
        printf("Usage: %s file\n", av[0]);
        return (0);
    }
    wrapper.prog_name = av[0];

	struct s_woody_file file_to_pack;
	if (fill_file(av[1], &file_to_pack))
	{
		perror(av[0]);
		return (1);
	}
	debug_print_file(&file_to_pack);

	struct s_woody_file new_file;
	if (create_file(&new_file, &file_to_pack))
	{
		perror(av[0]);
		return (1);
	}	
	debug_print_file(&new_file);
	close_file(&file_to_pack);
	close_file(&new_file);
}