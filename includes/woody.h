/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   woody.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niragne <niragne@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/24 12:47:05 by niragne           #+#    #+#             */
/*   Updated: 2020/01/04 15:26:36 by niragne          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WOODY_H
# define WOODY_H

# ifdef __APPLE__
#	include "libft.h"
# endif

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
# define DEFAULT_PAYLOAD "./payload"

# ifndef LIBFT_H
# 	define ft_memcpy memcpy
# 	define ft_memcmp memcmp
#	define ft_strcmp strcmp
# 	define ft_strlen strlen
# 	define ft_printf printf
# 	define ft_dprintf dprintf
# 	define ft_sprintf sprintf
# endif

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
	struct s_payload		payload;
	void*					old_entry;
	void*					new_entry;
	int						to_encrypt : 1;		//Should the file be packed or should just the payload be injected
	uint8_t*				key;				//RC4 key stored on 64 bits for convenience
};

/* 
** Debug
*/
void	debug_print_stat(struct stat *st);
void	debug_print_header64(Elf64_Ehdr *hdr);
void	debug_print_file(struct s_woody_file *file);

/*
** File handling
*/
int		fill_file(char* path, struct s_woody_file* buf);
int		create_file(struct s_woody_file* buf, struct s_woody_file* original);
void	fill_file_type(struct s_woody_file* buf);
int		close_file(struct s_woody_file* buf);
int		build_header_elf64(struct s_woody* wrapper);

/*
** Payload handling
*/
int		get_payload(struct s_woody* wrapper);
int		inject_payload(struct s_woody* wrapper);

/*
** ELF parsing
*/
Elf64_Shdr* find_section(void *ptr_elf, char *query);
Elf64_Phdr* find_codecave(void *ptr_elf, int fsize, int *offset, int *cave_size);

/*
** RC4
*/
void		rc4_crypt(char* mem, size_t size, uint8_t key[8]);
uint64_t	generate_key(void);

/*
** Misc
*/
void		replace_pattern(uint64_t pattern, char* mem, size_t size, uint64_t replace);

#endif