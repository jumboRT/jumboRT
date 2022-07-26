#include "world.h"

t_vec		f_bxdf_diffuse(t_diffuse_bsdf bsdf, t_vec wo, t_vec wi);
t_vec		f_bxdf_specular(t_specular_bsdf bsdf, t_vec wo, t_vec wi);
t_vec		f_bxdf_refractive(t_refractive_bsdf bsdf, t_vec wo, t_vec wi);

t_vec
	f_bsdf(const GLOBAL t_world *world, uint32_t bsdf, t_vec wo, t_vec wi)
{
	t_vec		result;
	uint32_t	index;

	index =
}
