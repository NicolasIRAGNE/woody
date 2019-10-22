#include "woody.h"
#include <stdio.h>

int main(int ac, char **av)
{
    Elf64_Ehdr* header;
    FILE* file;
    struct stat st;
    int fd;
    int new_fd;
    char* ptr;

    fd = open(av[1], O_RDONLY);
    if (ac != 2)
    {
        printf("Usage: %s file\n", av[0]);
        return (0);
    }
    if (fd <= 0)
    {
        perror(av[0]);
        return (0);
    }
    fstat(fd, &st);
    ptr = mmap(NULL, st.st_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
    if (ptr == MAP_FAILED)
    {
        perror(av[0]);
        return (0);
    }
    header = (Elf64_Ehdr*) ptr;
    debug_print_header64(header);

    char *new_file;
    new_file = malloc(sizeof(PACKED_PREFIX) + sizeof(PACKED_SUFFIX) + strlen(av[1]));
    sprintf(new_file, "%s%s%s", PACKED_PREFIX, av[1], PACKED_SUFFIX);
    new_fd = open(new_file, O_WRONLY | O_CREAT | O_TRUNC);
    if (new_fd < 0)
    {
        perror(av[0]);
        return (0);
    }
    close(fd);
    close(new_fd);
    printf("bonjour\n");
}