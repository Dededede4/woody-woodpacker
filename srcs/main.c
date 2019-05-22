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

/*
 * function to open binary and get its size, doesn't map it yet
 * you have to close fd yourself
 * return file fd or -1 on error, take pointer on int for size
 */
static int	open_file(char const *name, size_t *size)
{
	int	fd;

	fd = open(name, O_RDONLY);
	if (fd < 0) {
		perror("open");
	} else {
		*size = lseek(fd, 0, SEEK_END);
		if ((*size = lseek(fd, 0, SEEK_END)) < 0) {
			perror("lseek");
			close(fd);
			fd = -1;
		}
	}
	return fd;
}

/*
 * function to write a modified (or not) copy of binary into woody
 * return -1 on error, 0 on success
 */
static int	write_file(char const *map, size_t const size)
{
	int	fd;

	fd = open("woody", O_RDWR | O_CREAT, 0755);
	if (fd < 0) {
		perror("open");
		return -1;
	}
	write(fd, map, size);
	close(fd);
	return 0;
}

static int	dump_woody(void *map, int const fd, size_t const size)
{
	if (write_file(map, size) < 0) {
		munmap(map, size);
		close(fd);
		return 1;
	} else {
		munmap(map, size);
		close(fd);
	}
	return 0;
}

int		main(int argc, char **argv)
{
	size_t		size;
	int		fd;
	char		*map;
	Elf64_Ehdr	*hdr;

	if (argc != 2) {
		dprintf(STDERR_FILENO, "Error: Invalid number of arguments\n");
		dprintf(STDERR_FILENO, "Usage: %s <filename>\n", argv[0]);
		return 1;
	}

	fd = open_file(argv[1], &size);
	if (fd < 0)
		return 1;

	map = (char *)mmap(0, size, PROT_READ | PROT_WRITE, MAP_FILE | MAP_PRIVATE, fd, 0);
	if (map == MAP_FAILED) {
		perror("mmap");
		return (1);
	}

	hdr = (Elf64_Ehdr *)map;
	parse_ph_64(hdr, size);
	return dump_woody(map, fd, size);
}
