/* ************************************************************************** */
/*                                                                            */
/*                                                           :      .         */
/*   ft_putstr_fd.c                                   -=-:::+*+:-+*#.         */
/*                                                :-:::+#***********----:     */
/*   By: csteenvo <csteenvo@student.codam.n>        .:-*#************#-       */
/*                                                 :=+*+=+*********####+:     */
/*   Created: 2022/05/10 11:58:32 by csteenvo     ..     +**=-=***-           */
/*   Updated: 2022/05/10 11:58:32 by csteenvo            :      ..            */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <unistd.h>

int
	ft_putstr_fd(const char *str, int fd)
{
	if (FT_SAFE >= 1 && str == NULL)
		return (-1);
	return (write(fd, str, ft_strlen(str)));
}
