/* ************************************************************************** */
/*                                                                            */
/*                                                           :      .         */
/*   ft_lstclear_bonus.c                              -=-:::+*+:-+*#.         */
/*                                                :-:::+#***********----:     */
/*   By: csteenvo <csteenvo@student.codam.n>        .:-*#************#-       */
/*                                                 :=+*+=+*********####+:     */
/*   Created: 2022/05/10 11:58:24 by csteenvo     ..     +**=-=***-           */
/*   Updated: 2022/05/10 11:58:24 by csteenvo            :      ..            */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void
	ft_lstclear(t_list **lst, void (*del)(void *))
{
	if (*lst == NULL)
		return ;
	ft_lstclear(&(*lst)->next, del);
	ft_lstdelone(*lst, del);
	*lst = NULL;
}
