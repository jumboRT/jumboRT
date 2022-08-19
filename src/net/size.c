#include "net.h"

uint64_t
	rt_sizesr(struct s_send_results packet)
{
	return (32 + packet.zsize);
}
