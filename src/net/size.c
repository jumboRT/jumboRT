#include "net.h"

uint64_t
	rt_sizesr(struct s_send_results packet)
{
	return (32 + packet.zsize);
}

uint64_t
	rt_sizecjr(struct s_cjob_request packet)
{
	return (76 + packet.scene_file.len + packet.scene_key.len);
}
