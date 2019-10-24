#include "woody.h"
#include <stdio.h>

int    fill_file(char* path, struct s_woody_file* buf)
{
	/// Fill a s_woody_file with relevant information from a given path.
	/// Returns zero on success and non-zero on error.
	/// The file type is irrelevant to the return value. It needs
	/// to be checked manually if needed.
    int fd;

	buf->path = path;
    fd = open(path, O_RDONLY);
    if (fd < 0)
        return (1);
    buf->fd = fd;
	buf->is_open = 1;
    if (fstat(fd, &buf->st) == -1)
		return (1);
    buf->ptr = mmap(NULL, buf->st.st_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
    if (buf->ptr == MAP_FAILED)
        return (1);
	fill_file_type(buf);
    return (0);
}

int		create_file(struct s_woody_file* buf, struct s_woody_file* original)
{
	int fd;

	buf->path = malloc(sizeof(PACKED_SUFFIX) + sizeof(PACKED_PREFIX) + ft_strlen(original->path));
    ft_sprintf(buf->path, "%s%s%s", PACKED_PREFIX, original->path, PACKED_SUFFIX);
	fd = open(buf->path, O_WRONLY | O_TRUNC | O_CREAT, original->st.st_mode & RWX_UGO);
    if (fd < 0)
        return (1);
    buf->fd = fd;
	buf->is_open = 1;
	buf->st = original->st;
    // buf->ptr = mmap(NULL, buf->st.st_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
    buf->ptr = NULL;
	buf->type = original->type;
	if (buf->ptr == MAP_FAILED)
        return (1);
    return (0);
}

void	fill_file_type(struct s_woody_file* buf)
{
	uint32_t magic;

	magic = *(uint32_t*)(buf->ptr);
	if (!ft_memcmp(&magic, ELFMAG, sizeof(magic)))
	{
		buf->header_union.elf64 = (Elf64_Ehdr*)buf->ptr;
		buf->type = E_TYPE_ELF64;
	}
	else
		buf->type = E_TYPE_UNKNOWN;
}

int		close_file(struct s_woody_file* buf)
{
	if (buf->is_open)
	{
		close(buf->fd);
		buf->is_open = 0;
		buf->fd = -1;
		return (0);
	}
	return (0);
}

int		build_header_elf64(struct s_woody* wrapper)
{
	if ((wrapper->new_file->header_union.elf64 = malloc(sizeof(Elf64_Ehdr))) == 0)
	{
		perror(wrapper->prog_name);
		return (1);
	}
	memcpy(wrapper->new_file->header_union.elf64, wrapper->file_to_pack->header_union.elf64, sizeof(Elf64_Ehdr));
	return (0);
}