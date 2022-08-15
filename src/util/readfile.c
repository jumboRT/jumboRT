#include "util.h"

#if !defined RT_WINDOWS
#include <ft_printf.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

static int
	rt_open(const char *path, int oflag)
{
	int	fd;

	while (1)
	{
		fd = open(path, oflag);
		if (fd >= 0)
			break ;
		if (fd < 0 && errno == EINTR)
			continue;
		return (-1);
	}
	return (fd);
}

static ssize_t
	rt_read(int fildes, void *buf, size_t nbyte)
{
	ssize_t	read_size;
	size_t	bytes_read;
	char	*buf_cpy;

	buf_cpy = buf;
	bytes_read = 0;
	while (bytes_read < nbyte)
	{
		read_size = read(fildes, &buf_cpy[bytes_read], nbyte - bytes_read);
		if (read_size < 0 && errno == EINTR)
			continue;
		else if (read_size < 0)
			return (-1);
		if (read_size == 0)
			return (bytes_read);
		bytes_read += read_size;
		buf_cpy += read_size;
	}
	return (bytes_read);
}

static char
	*rt_readfile_fd_end(char *buffer, size_t index, size_t size)
{
	if (index + 1 >= size)
		buffer = rt_realloc(buffer, size, size + 1);
	buffer[index] = '\0';
	return (buffer);
}

static char
	*rt_readfile_fd(int fd, char **error, size_t *index)
{
	char	*buffer;
	size_t	size;
	ssize_t	read_size;

	size = RT_BUFFER_SIZE;
	buffer = rt_malloc(RT_BUFFER_SIZE);
	while (1)
	{
		if (*index + RT_BUFFER_SIZE >= size)
			buffer = rt_realloc(buffer, size, size * 2);
		if (*index + RT_BUFFER_SIZE >= size)
			size = size * 2;
		read_size = rt_read(fd, &buffer[*index], RT_BUFFER_SIZE);
		if (read_size < 0)
			return (rt_free(buffer), ft_asprintf(error,
					"Failed to read from fd %d: '%s'", fd, strerror(errno)),
				NULL);
		else if (read_size == 0)
			return (rt_readfile_fd_end(buffer, *index, size));
		*index += read_size;
	}
	return (NULL);
}

char
	*rt_readfile(const char *path, char **error, size_t *size)
{
	int		fd;
	size_t	size_int;

	if (size == NULL)
		size = &size_int;
	*size = 0;
	fd = rt_open(path, O_RDONLY);
	if (fd < 0)
	{
		ft_asprintf(error, "Unable to open '%s': %s", path, strerror(errno));
		return (NULL);
	}
	return (rt_readfile_fd(fd, error, size));
}
#endif

