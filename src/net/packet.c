/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   packet.c                                       #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 12:02:11 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/25 12:02:11 by csteenvo                                 */
/*                                                                            */
/* ************************************************************************** */

#include "net.h"

#if RT_BONUS
# include "util.h"

void
	rt_packet_destroy(struct s_packet *packet)
{
	rt_free(packet->data);
}

void
	rt_packet_create(struct s_packet *packet, t_uint64 data_size,
					t_uint8 type, void *data)
{
	packet->size = data_size;
	packet->type = type;
	packet->data = data;
}
#endif
