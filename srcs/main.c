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

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include "elf.h"
#include <string.h>

int				main(int argc, char **argv)
{
	const		unsigned char	data[] = {0x9C , 0x50 , 0x57 , 0x56 , 0x54 , 0x52 , 0x51 , 0xBF
										, 0x01 , 0x00 , 0x00 , 0x00 , 0x48 , 0x8D , 0x35 , 0x0C  , 0x00 , 0x00 , 0x00 , 0xBA , 0x10 , 0x00 , 0x00 , 0x00  
										, 0x48 , 0x89 , 0xF8 , 0x0F , 0x05 , 0xEB , 0x10 , 0x2E  , 0x2E , 0x2E , 0x2E , 0x57 , 0x4F , 0x4F , 0x44 , 0x59  
										, 0x2E , 0x2E , 0x2E , 0x2E , 0x2E , 0x0A , 0x00 , 0x48  , 0x8B , 0x05 , 0x33 , 0x00 , 0x00 , 0x00 , 0x48 , 0x8B  
										, 0x0D , 0x34 , 0x00 , 0x00 , 0x00 , 0x48 , 0x8B , 0x15  , 0x1D , 0x00 , 0x00 , 0x00 , 0x48 , 0x01 , 0xC1 , 0x30  
										, 0x10 , 0x48 , 0xC1 , 0xCA , 0x08 , 0x48 , 0xFF , 0xC0  , 0x48 , 0x39 , 0xC8 , 0x75 , 0xF2 , 0x59 , 0x5A , 0x5C  
										, 0x5E , 0x5F , 0x58 , 0x9D , 0xE9 , 0x67 , 0xFA , 0xDF  , 0xFF , 0x4A , 0x8E , 0x24 , 0x05 , 0xC8 , 0x4C , 0xE0  
										, 0x39 , 0xE0 , 0x03 , 0x40 , 0x00 , 0x00 , 0x00 , 0x00  , 0x00 , 0xA2 , 0x01 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00  
										, 0x00                       , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00  
										, 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00  , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00  };
	ssize_t		size;
	int			fd;
	char		*map;

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
	Elf64_Ehdr	*hdr;

	hdr = (Elf64_Ehdr *)map;
	printf("%llx\n", hdr->e_entry);
	hdr->e_entry = 0x201070;

	int fd2 = open("dest", O_RDWR|O_CREAT, 0755);
	if (fd2) {
		write(fd2, map, size);
		write(fd2, &data[0], 145);
		close(fd2);
	}

	munmap(map, size);
	close(fd);
	return (0);
}
