/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heynard <heynard@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/15 13:07:54 by heynard           #+#    #+#             */
/*   Updated: 2019/05/15 18:03:06 by heynard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "woody.h"
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

int				main(int argc, char **argv)
{
	size_t		size;
	int		fd;
	char		*map;
	Elf64_Ehdr	*hdr;

	if (argc != 2) {
		dprintf(STDERR_FILENO, "Error: Invalid number of arguments\n");
		dprintf(STDERR_FILENO, "Usage: %s <filename>\n", argv[0]);
		return (1);
	}

	if ((fd = open(argv[1], O_RDONLY)) < 0) {
		perror("open");
		return (1);
	}
	if ((size = lseek(fd, 0, SEEK_END)) < 0) {
		perror("lseek");
		return (1);
	}

	map = (char *)mmap(0, size, PROT_READ | PROT_WRITE, MAP_FILE | MAP_PRIVATE, fd, 0);
	if (map == MAP_FAILED) {
		perror("mmap");
		return (1);	
	}

	printf("%hhx%c%c%c\n", map[0], map[1], map[2], map[3]);

	hdr = (Elf64_Ehdr *)map;
	parse_ph_64(hdr, size);

	printf("\n Entry point : %llx\n", hdr->e_entry);

	int fd2 = open("dest", O_RDWR|O_CREAT, 0755);
	if (fd2) {
		write(fd2, map, size);
		close(fd2);
	}

	munmap(map, size);
	close(fd);
	return (0);
}
