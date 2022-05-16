/* ************************************************************************** */
/*                                                                            */
/*                                                           :      .         */
/*   ft_lstiter_bonus.c                               -=-:::+*+:-+*#.         */
/*                                                :-:::+#***********----:     */
/*   By: csteenvo <csteenvo@student.codam.n>        .:-*#************#-       */
/*                                                 :=+*+=+*********####+:     */
/*   Created: 2022/05/10 11:58:25 by csteenvo     ..     +**=-=***-           */
/*   Updated: 2022/05/10 11:58:25 by csteenvo            :      ..            */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void
	ft_lstiter(t_list *lst, void (*f)(void *))
{
	if (lst == NULL)
		return ;
	f(lst->content);
	ft_lstiter(lst->next, f);
}
