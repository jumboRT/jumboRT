/* ************************************************************************** */
/*                                                                            */
/*                                                           :      .         */
/*   ft_lstnew_bonus.c                                -=-:::+*+:-+*#.         */
/*                                                :-:::+#***********----:     */
/*   By: csteenvo <csteenvo@student.codam.n>        .:-*#************#-       */
/*                                                 :=+*+=+*********####+:     */
/*   Created: 2022/05/10 11:58:27 by csteenvo     ..     +**=-=***-           */
/*   Updated: 2022/05/10 11:58:27 by csteenvo            :      ..            */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "../../include/memory.h"
#include <stdlib.h>

t_list
	*ft_lstnew(void *content)
{
	t_list	*lst;

	lst = sh_safe_malloc(sizeof(*lst));
	if (lst == NULL)
		return (NULL);
	lst->content = content;
	lst->next = NULL;
	return (lst);
}
