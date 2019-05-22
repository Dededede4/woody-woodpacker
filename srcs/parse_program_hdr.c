#include "woody.h"

static void	print_phtype(Elf64_Phdr *hdr) {
	switch (hdr->p_type) {
		case PT_NULL:
			printf("PT_NULL\n");
			break ;
		case PT_LOAD:
			printf("PT_LOAD\n");
			break ;
		case PT_DYNAMIC:
			printf("PT_DYNAMIC\n");
			break ;
		case PT_INTERP:
			printf("PT_INTERP\n");
			break ;
		case PT_NOTE:
			printf("PT_NOTE\n");
			break ;
		case PT_SHLIB:
			printf("PT_SHLIB\n");
			break ;
		case PT_PHDR:
			printf("PT_PHDR\n");
			break ;
		case PT_TLS:
			printf("PT_TLS\n");
			break ;
		case PT_LOOS:
			printf("PT_LOOS\n");
			break ;
		case PT_HIOS:
			printf("PT_HIOS\n");
			break ;
		case PT_LOPROC:
			printf("PT_LOPROC\n");
			break ;
		case PT_HIPROC:
			printf("PT_HIPROC\n");
			break ;
	}
}

int	parse_ph_64(Elf64_Ehdr *map, size_t size)
{
	Elf64_Phdr	*tmp;
	int		i;

	if (map->e_phnum > 0) {
		tmp = (Elf64_Phdr *)((void *)map + map->e_phoff);
		for (i = 0; i < map->e_phnum; i++) {
			print_phtype(tmp);

			if (tmp->p_type == PT_NOTE) {
				tmp->p_vaddr = 0x00;
				tmp->p_paddr = 0x00;
			}

			tmp++;
		}
	}
	return 0;
}
