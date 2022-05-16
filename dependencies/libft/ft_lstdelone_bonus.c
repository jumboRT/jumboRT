/* ************************************************************************** */
/*                                                                            */
/*                                                           :      .         */
/*   ft_lstdelone_bonus.c                             -=-:::+*+:-+*#.         */
/*                                                :-:::+#***********----:     */
/*   By: csteenvo <csteenvo@student.codam.n>        .:-*#************#-       */
/*                                                 :=+*+=+*********####+:     */
/*   Created: 2022/05/10 11:58:24 by csteenvo     ..     +**=-=***-           */
/*   Updated: 2022/05/10 11:58:24 by csteenvo            :      ..            */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

void
	ft_lstdelone(t_list *lst, void (*del)(void *))
{
	del(lst->content);
	free(lst);
}
