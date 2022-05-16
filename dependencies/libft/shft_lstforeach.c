/* ************************************************************************** */
/*                                                                            */
/*                                                           :      .         */
/*   shft_lstforeach.c                                -=-:::+*+:-+*#.         */
/*                                                :-:::+#***********----:     */
/*   By: csteenvo <csteenvo@student.codam.n>        .:-*#************#-       */
/*                                                 :=+*+=+*********####+:     */
/*   Created: 2022/05/10 11:58:51 by csteenvo     ..     +**=-=***-           */
/*   Updated: 2022/05/10 11:58:51 by csteenvo            :      ..            */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void
	ft_lstforeach(const t_list *lst, void (*proc)(void *, void *), void *param)
{
	while (lst)
	{
		proc(lst->content, param);
		lst = lst->next;
	}
}
