#include "woody.h"

int		cave_len(char *cave)
{
	int	size;

	size = 0;
	while (cave[size] == 0)
		size++;
	return size;
}
