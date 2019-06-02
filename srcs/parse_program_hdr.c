#include "woody.h"

static int		inject_code(Elf64_Ehdr *map, int maxaddr, int maxoff)
{
	int		retval;
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
	ft_memcpy((void *)map + maxoff, binary_virus, size);
	retval = 0;
	if (munmap(binary_virus, size) < 0) {
		perror("munmap");
		retval = -1;
	}
	if (close(fd) < 0) {
		perror("close");
		retval = -1;
	}
	return retval;
}

static void		encrypt_main(void *bin, size_t len, unsigned char *secret)
{
	size_t i = 0;

	while (i < len)
	{
		((unsigned char*)bin)[i] = ((unsigned char*)bin)[i] ^ secret[i % 8];
		i++;
	}
}

static int		check_file_ident(Elf64_Ehdr *map, long long int const size)
{
	(void)size;
	if (map->e_ident[EI_MAG0] == ELFMAG0
	 && map->e_ident[EI_MAG1] == ELFMAG1
	 && map->e_ident[EI_MAG2] == ELFMAG2
	 && map->e_ident[EI_MAG3] == ELFMAG3
	 && map->e_ident[EI_CLASS] == ELFCLASS64) {
		if (map->e_ehsize + (map->e_phentsize * map->e_phnum)
		 + (map->e_shentsize * map->e_shnum < size))
			return 0;
	}
	dprintf(STDERR_FILENO, "Error: invalid file format\n");
	return -1;
}


Elf64_Shdr *get_text_segment(Elf64_Ehdr *map)
{
	Elf64_Shdr *tmp;
	char *tab;
	int i;


	if (map->e_shoff > 0)
	{
		tmp = (Elf64_Shdr * )((void*) map + map->e_shoff);
		tab = (char*)(((void*)map) + (tmp[map->e_shstrndx]).sh_offset);
		for (i = 0; i < map->e_shnum; i++)
		{
			if (ft_strcmp(".text", tab + tmp->sh_name) == 0)
			{
				return (tmp);
			}
			tmp++;
		}
		
	}
	return (NULL);
}


int	parse_ph_64(Elf64_Ehdr *map, long long int const size, unsigned char *secret)
{
	Elf64_Phdr	*tmp;
	unsigned int	maxaddr;
	int		maxoff;
	int		i;
	Elf64_Shdr *segment;
	
	(void)secret;
	
	if (check_file_ident(map, size) < 0)
		return -1;
	maxaddr = 0x0;
	maxoff = 0x0;
	if (map->e_phnum > 0) {
		tmp = (Elf64_Phdr *)((void *)map + map->e_phoff);
		for (i = 0; i < map->e_phnum; i++) {
			printf(".\n");
			if (maxaddr < tmp->p_vaddr + tmp->p_memsz) {
				if ((tmp->p_flags & PF_X) > 0) {
					maxaddr = tmp->p_vaddr + tmp->p_memsz;
					maxoff = tmp->p_offset + tmp->p_filesz;
				}
			}
			tmp++;
		}
	}
	if (maxaddr == 0)
		return -1;
	if (NULL == (segment = get_text_segment(map)))
		return -1;
	printf("On encrypte à partir de %lu\n", segment->sh_offset);
	encrypt_main(((void*)map) + segment->sh_offset, segment->sh_size, secret);
	return inject_code(map, maxaddr, maxoff);
}
