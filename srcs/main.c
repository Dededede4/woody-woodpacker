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

static int	write_file(char const *map, long long int const size)
{
	int	fd;

	fd = open("woody", O_RDWR|O_CREAT|O_TRUNC, 0755);
	if (fd < 0) {
		perror("open");
		return -1;
	}
	write(fd, map, size);
	close(fd);
	return 0;
}

static int	dump_woody(void *map, int const fd, long long int const size)
{
	int	retval1;
	int	retval2;
	int	retval3;

	retval1 = write_file(map, size);
	retval2 = munmap(map, size);
	if (retval2 < 0)
		perror("munmap");
	retval3 = close(fd);
	if (retval3 < 0)
		perror("close");
	if (retval1 || retval2 || retval3)
		return 1;
	return 0;
}

int		open_file(char const *name, long long int *size, int const mod)
{
	int	fd;

	fd = open(name, mod);
	if (fd < 0) {
		perror("open");
	} else {
		*size = lseek(fd, 0, SEEK_END);
		if (*size < 0) {
			perror("lseek");
			close(fd);
			fd = -1;
		}
	}
	return fd;
}

void gen_secret(unsigned char *secret)
{
	int fd;


	if((fd = open("/dev/random", O_RDONLY)) < 0)
		exit(0);
	read(fd, secret, 8);
}

int		main(int argc, char **argv)
{
	long long int	size;
	int		fd;
	char		*map;
	unsigned char secret[8];

	if (argc != 2) {
		dprintf(STDERR_FILENO, "Error: Invalid number of arguments\n");
		dprintf(STDERR_FILENO, "Usage: %s <filename>\n", argv[0]);
		return 1;
	}

	ft_putstr("aaa");
	gen_secret(secret);
	ft_putstr("bbb\n");
	fd = open_file(argv[1], &size, O_RDONLY);
	if (fd < 0)
		return 1;

	map = (char *)mmap(0, size, PROT_READ | PROT_WRITE, MAP_FILE | MAP_PRIVATE, fd, 0);
	if (map == MAP_FAILED) {
		perror("mmap");
		return (1);
	}
	if (parse_ph_64((Elf64_Ehdr *)map, size, secret) < 0)
		return -1;
	return dump_woody(map, fd, size);
}
