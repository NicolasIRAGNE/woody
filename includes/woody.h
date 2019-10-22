#ifndef WOODY_H
# define WOODY_H

# include <elf.h>
# include <sys/mman.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <unistd.h>
# include <fcntl.h>
# include <stdlib.h>
# include <string.h>
# define PACKED_SUFFIX "_packed"
# define PACKED_PREFIX ""
# define RWX_UGO (S_IRWXU | S_IRWXG | S_IRWXO)

# define ELF_MAGIC (ELFMAG0 | (ELFMAG1 << 8) | (ELFMAG2 << 16) | (ELFMAG3 << 24))

enum e_type
{
	E_TYPE_UNKNOWN,
	E_TYPE_ELF64,
	E_TYPE_ELF32,
	E_TYPE_MACHO
};

struct s_payload //Payload to inject in the new binary
{
	uint8_t*    data; //Bytecode
	size_t      size; //Size of the bytecode
};

struct s_woody_file //Struct to handle files
{
	char*			path;			//The relative path to the file
	void*			ptr;			//Pointer to the mapped file contents
	int				fd;				//File descriptor of the file
	int				is_open : 1;	//If the fd is still open
	enum e_type		type;
	struct stat		st;				//Stat
	union
	{
		Elf64_Ehdr	*elf64;
	} header_union;					//Union containing all the supported headers
};

struct s_woody //General wrapper used for the executable
{
	char*					prog_name;			//Name of the executable
	struct s_woody_file*	file_to_pack;		//The file to be injected
	struct s_woody_file*	file_to_inject;		//The file where the payload is extracted from
	struct s_woody_file*	new_file;			//The new file to be created
	int						to_encrypt : 1;		//Should the file be packed or should just the payload be injected 
};


void	debug_print_stat(struct stat *st);
void	debug_print_header64(Elf64_Ehdr *hdr);
void	debug_print_file(struct s_woody_file *file);
int		fill_file(char* path, struct s_woody_file* buf);
int		create_file(struct s_woody_file* buf, struct s_woody_file* original);
void	fill_file_type(struct s_woody_file* buf);
int		close_file(struct s_woody_file* buf);

#endif