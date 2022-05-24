#include "material.h"
#include "scene.h"
#include <math.h>

const t_material_vt
	*lambertian_vt(void)
{
	static const t_material_vt	vt = {
		lambertian_scatter,
		lambertian_destroy
	};

	return (&vt);
}

/* TODO: maybe use random_hvec instead */
int
	lambertian_scatter(t_material *mat, t_hit *hit, t_scatter *scatter, t_thread_ctx *ctx)
{
	t_lambertian	*lambertian;
	t_vec			dir;

	lambertian = (t_lambertian *) mat;
	scatter->attenuation = lambertian->albedo;
	scatter->scattered.pos = hit->pos;
	dir = vec_add(hit->normal, rt_random_svec(&ctx->seed));
	if (vec_mag(dir) < 0.001)
		dir = hit->normal;
	scatter->scattered.dir = vec_norm(dir);
	return (1);
}

void
	lambertian_destroy(t_material *mat)
{
	rt_free(mat);
}
