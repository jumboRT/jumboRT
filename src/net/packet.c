#include "net.h"

#include "util.h"

void
	packet_destroy(struct s_packet *packet)
{
	rt_free(packet->data);
}

struct s_packet
	packet_create(uint64_t data_size, uint8_t type, void *data)
{
	struct s_packet	packet;

	packet.size = data_size;
	packet.type = type;
	packet.data = data;
	return (packet);
}
