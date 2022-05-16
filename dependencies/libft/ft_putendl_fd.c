/* ************************************************************************** */
/*                                                                            */
/*                                                           :      .         */
/*   ft_putendl_fd.c                                  -=-:::+*+:-+*#.         */
/*                                                :-:::+#***********----:     */
/*   By: csteenvo <csteenvo@student.codam.n>        .:-*#************#-       */
/*                                                 :=+*+=+*********####+:     */
/*   Created: 2022/05/10 11:58:31 by csteenvo     ..     +**=-=***-           */
/*   Updated: 2022/05/10 11:58:31 by csteenvo            :      ..            */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int
	ft_putendl_fd(const char *str, int fd)
{
	int	rc;
	int	nwrite;

	if (FT_SAFE >= 1 && str == NULL)
		return (-1);
	nwrite = 0;
	rc = ft_putstr_fd(str, fd);
	if (rc < 0)
		return (-1);
	nwrite = rc;
	rc = ft_putchar_fd('\n', fd);
	if (rc < 0)
		return (-1);
	return (nwrite + rc);
}
