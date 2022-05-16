/* ************************************************************************** */
/*                                                                            */
/*                                                           :      .         */
/*   ft_lstadd_back_bonus.c                           -=-:::+*+:-+*#.         */
/*                                                :-:::+#***********----:     */
/*   By: csteenvo <csteenvo@student.codam.n>        .:-*#************#-       */
/*                                                 :=+*+=+*********####+:     */
/*   Created: 2022/05/10 11:58:23 by csteenvo     ..     +**=-=***-           */
/*   Updated: 2022/05/10 11:58:23 by csteenvo            :      ..            */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void
	ft_lstadd_back(t_list **lst, t_list *new)
{
	if (*lst != NULL)
		lst = &ft_lstlast(*lst)->next;
	*lst = new;
}
