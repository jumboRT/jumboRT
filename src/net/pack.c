#include "net.h"

void
	*rt_packu64(void *dst, uint64_t i)
{
	unsigned char *buf;

	buf = dst;
	*buf++ = i >> 56;
	*buf++ = i >> 48;
	*buf++ = i >> 40;
	*buf++ = i >> 32;
	*buf++ = i >> 24;
	*buf++ = i >> 16;
	*buf++ = i >> 8;
	*buf++ = i;
	return (buf);
}

void
	*rt_packhs(void *dst, struct s_handshake packet)
{
	unsigned char *buf;

	buf = dst;
	*buf++ = packet.client_type;
	return (buf);
}
