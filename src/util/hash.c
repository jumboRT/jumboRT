/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   hash.c                                         #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 12:02:16 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/25 12:02:16 by csteenvo                                 */
/*                                                                            */
/* ************************************************************************** */

#include "util.h"

unsigned int
	rt_hash(const char *str)
{
	unsigned int	basis;	
	size_t			index;

	index = 0;
	basis = 2166136261;
	while (str[index] != '\0')
	{
		basis = (basis ^ str[index]) * 16777619;
		++index;
	}
	return (basis);
}
