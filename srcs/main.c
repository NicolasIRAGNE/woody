#include "woody.h"
#include <stdio.h>
#include <time.h>

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