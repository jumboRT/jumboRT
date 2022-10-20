#include "util.h"

#if defined RT_WINDOWS
# include <stdio.h>
# include <errno.h>
# include <string.h>
# include <stdlib.h>
# define READ_SIZE 1000000

static int
	rt_readfile_loop(char **str, FILE *fd, size_t *len)
{
	ssize_t	n;

	*str = realloc(*str, *len + READ_SIZE);
	rt_assert(*str != NULL, "str is null");
	n = fread(*str + *len, 1, READ_SIZE, fd);
	if (n > 0)
		*len += n;
	if (n == 0 && feof(fd))
		return (0);
	else if (n == 0)
		rt_assert(0, "bad read");
	return (1);
}

char
	*rt_readfile(const char *path, char **error, size_t *len)
{
	char	*str;
	FILE	*fd;
	size_t	len2;
	ssize_t	n;

	if (len == NULL)
		len = &len2;
	(void) error;
	fd = fopen(path, "rb");
	rt_assert(fd != NULL, "fd is null");
	str = NULL;
	*len = 0;
	while (rt_readfile_loop(&str, fd, len))
		continue ;
	fclose(fd);
	str = realloc(str, *len + 1);
	str[*len] = '\0';
	return (str);
}

#endif
