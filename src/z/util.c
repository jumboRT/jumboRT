/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   util.c                                         #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 12:02:13 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/25 12:02:13 by csteenvo                                 */
/*                                                                            */
/* ************************************************************************** */

#include "z.h"

unsigned int
	reverse_bits(unsigned int x, unsigned int n)
{
	x = ((x & 0xAAAAAAAA) >> 1) | ((x & 0x55555555) << 1);
	x = ((x & 0xCCCCCCCC) >> 2) | ((x & 0x33333333) << 2);
	x = ((x & 0xF0F0F0F0) >> 4) | ((x & 0x0F0F0F0F) << 4);
	x = ((x & 0xFF00FF00) >> 8) | ((x & 0x00FF00FF) << 8);
	x = ((x & 0xFFFF0000) >> 16) | ((x & 0x0000FFFF) << 16);
	return (x >> (32 - n));
}
