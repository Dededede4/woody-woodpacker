#include "woody.h"
#include <byteswap.h>

       #include <sys/types.h>
	       #include <sys/stat.h>
	       #include <fcntl.h>

void replace_value(char *map, long long int const size, uint64_t searched, uint64_t value)
{
	long long int i;
	long long int stop;
	int founded;

	i = 0;
	founded = 0;
	stop = size - sizeof(uint64_t);
	while (i < stop)
	{
		if (*((uint64_t*)map) == searched)
		{
			if (searched == 0x4141414141414141)
			{
				printf("%lx --> %llx\n", searched, i + value);
				*((uint64_t*)map) = (i + value);
			}
			else
			{
				printf("%lx --> %lx\n", searched, value);
				*((uint64_t*)map) = (value);
			}
			founded++;
		}
		map++;
		i++;
	}
	printf("%lx Founded : %i\n", searched, founded);
}

static int		inject_code(Elf64_Ehdr *map, int maxaddr, int maxoff, uint64_t size_encrypted, uint64_t secret)
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
	//printf("-->%llu", map->e_entry);
	replace_value(binary_virus, size, 0x4141414141414141, size_encrypted); // Offset start
//	replace_value(binary_virus, size, 0x4242424242424242, maxoff); // Offset start
	replace_value(binary_virus, size, 0x4343434343434342, size_encrypted); // Longueur à déchiffrer
	replace_value(binary_virus, size, 0x4444444444444444, secret); // la clef de déchiffrement

	ft_memcpy((void *)map + maxoff, binary_virus, size);


	map->e_entry = maxaddr;

	retval = 0;
	fd = open("virus_transformed.bin", O_RDWR|O_CREAT|O_TRUNC, 0777);
	write(fd, binary_virus, size);
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
	size_t	rot = 0;
	size_t i = 0;

	(void)bin;
	(void)secret;
	while (i < len)
	{
		((unsigned char*)bin)[i] = ((unsigned char*)bin)[i] ^ secret[rot];
		i++;
		rot = rot == 7 ? 0 : rot + 1;
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
	Elf64_Phdr	*saved;
	Elf64_Shdr *segment;
	
	(void)secret;
	if (check_file_ident(map, size) < 0)
		return -1;
	maxaddr = 0x0;
	maxoff = 0x0;
	saved = NULL;
	if (map->e_phnum > 0) {
		tmp = (Elf64_Phdr *)((void *)map + map->e_phoff);
		for (i = 0; i < map->e_phnum; i++) {
			printf(".\n");
			if (maxaddr < tmp->p_vaddr + tmp->p_memsz) {
				if ((tmp->p_flags & PF_X) > 0) {
					tmp->p_flags |= PF_W;
					maxaddr = tmp->p_vaddr + tmp->p_memsz;
					maxoff = tmp->p_offset + tmp->p_filesz;
					saved = tmp;
				}
			}
			tmp++;
		}
	}
	if (maxoff > size) {
		dprintf(STDERR_FILENO, "Error: invalid file format\n");
		return -1;	
	}
	if (maxaddr == 0 || (cave_len((char *)map + maxoff) < PAYLOAD_SIZE)) {
		dprintf(STDERR_FILENO, "Error: not enough space to insert payload\n");
		return -1;
	}
	//if (maxaddr)
	if (NULL == (segment = get_text_segment(map)))
		return -1;
	printf("Secret :%lu\n", *((uint64_t*)secret));
	printf("On encrypte à partir de %lu\n", segment->sh_offset);
//	encrypt_main(((void*)map) + (maxoff - saved->p_filesz), saved->p_filesz, secret);
//	if(inject_code(map, maxaddr, maxoff, vaddr + segment->sh_offset, saved->p_filesz, *((uint64_t*)secret)) == -1)
	encrypt_main(((void*)map) + segment->sh_offset, maxoff - segment->sh_offset, secret);
	if(inject_code(map, maxaddr,  maxoff, maxoff - segment->sh_offset, *((uint64_t*)secret)) == -1)
		return -1;

	long long int	size2;

	open_file("virus.bin", &size2, O_RDONLY);


	saved->p_memsz += size2;
	saved->p_filesz += size2;
        segment->sh_size += size2;
	return (42);
}
