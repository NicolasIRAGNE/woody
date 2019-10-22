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

void    debug_print_stat(struct stat *st);
void    debug_print_header64(Elf64_Ehdr *hdr);

#endif