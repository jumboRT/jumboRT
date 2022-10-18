#include "bsdf.h"

#include "mat.h"

t_bsdf
	get_bsdf(const t_world_hit *hit)
{
	if (hit->is_volume)
		return (hit->mat->volume);
	else
		return (hit->mat->surface);
}

int32_t
	bxdf_is_perfspec(const GLOBAL t_bxdf *bxdf)
{
	return (bxdf->type == RT_BXDF_REFLECTIVE
		|| bxdf->type == RT_BXDF_TRANSMISSIVE
		|| bxdf->type == RT_BXDF_SPECULAR);
}
