#ifndef WOODY_H
# define WOODY_H

# include <stdlib.h>
# include <fcntl.h>
# include <sys/mman.h>
# include <unistd.h>
# include <errno.h>
# include <string.h>
# include <stdio.h>
# include <elf.h>

#include "../libft/includes/libft.h"

int	open_file(char const *name, long long int *size, int const mod);
int	parse_ph_64(Elf64_Ehdr *map, long long int const size);

#endif
