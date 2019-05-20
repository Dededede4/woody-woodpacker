#ifndef WOODY_H
# define WOODY_H

# include <stdio.h>	
# include <elf.h>

int	parse_ph_64(Elf64_Ehdr *map, size_t size);

#endif
