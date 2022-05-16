/* ************************************************************************** */
/*                                                                            */
/*                                                           :      .         */
/*   ft_lstmap_bonus.c                                -=-:::+*+:-+*#.         */
/*                                                :-:::+#***********----:     */
/*   By: csteenvo <csteenvo@student.codam.n>        .:-*#************#-       */
/*                                                 :=+*+=+*********####+:     */
/*   Created: 2022/05/10 11:58:26 by csteenvo     ..     +**=-=***-           */
/*   Updated: 2022/05/10 11:58:26 by csteenvo            :      ..            */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list
	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*new;
	void	*content;

	if (lst == NULL)
		return (NULL);
	content = f(lst->content);
	new = ft_lstnew(content);
	if (new == NULL)
		return (NULL);
	new->next = ft_lstmap(lst->next, f, del);
	if (new->next == NULL && lst->next != NULL)
	{
		ft_lstdelone(new, del);
		return (NULL);
	}
	return (new);
}
