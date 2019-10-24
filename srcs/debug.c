#include "woody.h"
#include <stdio.h>

void    debug_print_stat(struct stat *st)
{
    ft_printf("%d\n", st->st_size);
}

void    debug_print_header64(Elf64_Ehdr *hdr)
{
// typedef struct
// {
//   unsigned char	e_ident[EI_NIDENT];	/* Magic number and other info */
//   Elf64_Half	e_type;			/* Object file type */
//   Elf64_Half	e_machine;		/* Architecture */
//   Elf64_Word	e_version;		/* Object file version */
//   Elf64_Addr	e_entry;		/* Entry point virtual address */
//   Elf64_Off	e_phoff;		/* Program header table file offset */
//   Elf64_Off	e_shoff;		/* Section header table file offset */
//   Elf64_Word	e_flags;		/* Processor-specific flags */
//   Elf64_Half	e_ehsize;		/* ELF header size in bytes */
//   Elf64_Half	e_phentsize;		/* Program header table entry size */
//   Elf64_Half	e_phnum;		/* Program header table entry count */
//   Elf64_Half	e_shentsize;		/* Section header table entry size */
//   Elf64_Half	e_shnum;		/* Section header table entry count */
//   Elf64_Half	e_shstrndx;		/* Section header string table index */
// } Elf64_Ehdr;

    ft_printf("e_type = %d\n", hdr->e_type);
    ft_printf("e_machine = %d\n", hdr->e_machine);
    ft_printf("e_version = %d\n", hdr->e_version);
    ft_printf("e_entry = %p\n", hdr->e_entry);
    ft_printf("e_phoff = %d\n", hdr->e_phoff);
    ft_printf("e_shoff = %d\n", hdr->e_shoff);
    ft_printf("e_flags = %d\n", hdr->e_flags);
    ft_printf("e_ehsize = %d\n", hdr->e_ehsize);
    ft_printf("e_phentsize = %d\n", hdr->e_phentsize);
    ft_printf("e_phnum = %d\n", hdr->e_phnum);
    ft_printf("e_shentsize = %d\n", hdr->e_shentsize);
    ft_printf("e_shnum = %d\n", hdr->e_shnum);
    ft_printf("e_shstrndx = %d\n", hdr->e_shstrndx);
}

void	debug_print_file(struct s_woody_file *file)
{
	ft_printf("DEBUG: PRINTING FILE %s\n", file->path);
	ft_printf("ptr: %p\n", file->ptr);
	ft_printf("fd: %d\n", file->fd);
	ft_printf("is_open: %d\n", (file->is_open != 0));
	if (file->type == E_TYPE_ELF64)
	{
		ft_printf("type: elf64\n");
		debug_print_header64(file->header_union.elf64);
	}
	else if (file->type == E_TYPE_ELF32)
		ft_printf("type: elf32\n");
	else if (file->type == E_TYPE_UNKNOWN)
		ft_printf("type: unknown\n");
	else
		ft_printf("SOMETHING WENT TERRIBLY WRONG THIS SHOULDNT EVER BE PRINTED\n");
}

/*struct s_woody_file //Struct to handle binary files
{
	char*			path;			//The relative path to the file
	void*			ptr;			//Pointer to the mapped file contents
	int				fd;				//File descriptor of the file
	int				is_open : 1;	//If the fd is still open
	enum e_type		type;
	struct stat		st;				//Stat
	union
	{
		Elf64_Ehdr *elf64;
	} header_union;					//Union containing all the supported headers
};
*/