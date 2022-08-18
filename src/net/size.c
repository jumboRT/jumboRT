#include "net.h"

uint64_t
	rt_sizesr(struct s_send_results packet)
{
	return (16 + 24 * packet.count);
}
