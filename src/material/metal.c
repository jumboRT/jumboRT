#include "material.h"
#include "scene.h"
#include <math.h>

const t_material_vt
	*metal_vt(void)
{
	static const t_material_vt	vt = {
		metal_scatter,
		metal_destroy
	};

	return (&vt);
}

int
	metal_scatter(t_material *mat, t_ray in, t_hit *hit, t_scatter *scatter, t_thread_ctx *ctx)
{
	t_metal	*metal;
	t_vec	dir;

	(void) ctx;
	metal = (t_metal *) mat;
	scatter->attenuation = metal->albedo;
	scatter->scattered.pos = hit->pos;
	dir = vec_sub(in.dir, vec_scale(hit->normal, 2 * vec_dot(in.dir, hit->normal)));
	scatter->scattered.dir = vec_norm(vec_add(dir, vec_scale(rt_random_svec(&ctx->seed), metal->fuzzy)));
	return (vec_dot(scatter->scattered.dir, hit->normal) > 0);
}

void
	metal_destroy(t_material *mat)
{
	rt_free(mat);
}
