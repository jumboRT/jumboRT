#include "util.h"

#include <ft_printf.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

static int
	rt_open(const char *path, char **error, int flags, mode_t mode)
{
	int	fd;

	while (1)
	{
		fd = open(path, flags, mode);
		if (fd >= 0)
			break ;
		if (errno == EINTR)
			continue;
		ft_asprintf(error, "failed to open %s: %s", path, strerror(errno));
		return (-1);
	}
	return (fd);
}

static int
	rt_close(int fd, char **error)
{
	int	return_code;

	while (1)
	{
		return_code = close(fd);
		if (return_code == 0)
			break ;
		if (errno == EINTR)
			continue;
		ft_asprintf(error, "failed to close fd %d: %s", fd, strerror(errno));
		return (-1);
	}
	return (0);
}

static ssize_t
	rt_writefile_fd(int fd, char **error, const void *data, size_t size)
{
	const char	*buf_pos;
	ssize_t		nwritten;
	size_t		left;

	buf_pos = data;
	nwritten = 0;
	left = size;
	while (left > 0)
	{
		nwritten = write(fd, buf_pos, left);
		if (nwritten < 0)
		{
			if (errno == EINTR)
				continue;
			ft_asprintf(error, "failed to write to fd %d: %s", fd, strerror(errno));
			return (-1);
		}
		buf_pos += nwritten;
		left -= nwritten;
	}
	return (size);
}

ssize_t
	rt_writefile(const char *path, char **error, const void *data, size_t size)
{
	size_t	nwritten;
	int		fd;

	fd = rt_open(path, error, O_CREAT | O_TRUNC | O_WRONLY, 0664);
	if (fd < 0)
		return (-1);
	nwritten = rt_writefile_fd(fd, error, data, size);
	if (nwritten < 0)
		return (-1);
	if (rt_close(fd, error) < 0)
		return (-1);
	return (nwritten);
}
