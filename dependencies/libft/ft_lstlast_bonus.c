/* ************************************************************************** */
/*                                                                            */
/*                                                           :      .         */
/*   ft_lstlast_bonus.c                               -=-:::+*+:-+*#.         */
/*                                                :-:::+#***********----:     */
/*   By: csteenvo <csteenvo@student.codam.n>        .:-*#************#-       */
/*                                                 :=+*+=+*********####+:     */
/*   Created: 2022/05/10 11:58:26 by csteenvo     ..     +**=-=***-           */
/*   Updated: 2022/05/10 11:58:26 by csteenvo            :      ..            */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list
	*ft_lstlast(t_list *lst)
{
	if (lst == NULL || lst->next == NULL)
		return (lst);
	return (ft_lstlast(lst->next));
}
