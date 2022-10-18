#if RT_BONUS
#include "net.h"

#include "util.h"

void
	rt_packet_destroy(struct s_packet *packet)
{
	rt_free(packet->data);
}

void
	rt_packet_create(struct s_packet *packet, uint64_t data_size,
					uint8_t type, void *data)
{
	packet->size = data_size;
	packet->type = type;
	packet->data = data;
}
#endif
