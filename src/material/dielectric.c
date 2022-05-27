#include "material.h"
#include "scene.h"
#include <math.h>

static FLOAT
	reflectance(FLOAT cosine, FLOAT refidx)
{
	const FLOAT	r0 = (1 - refidx) / (1 + refidx);
	const FLOAT	r1 = r0 * r0;
	return (r1 + (1 - r1) * pow(1 - cosine, 5));
}

t_vec
	refract(t_vec v, t_vec n, FLOAT eoe)
{
	const FLOAT	cos_theta = fmin(vec_dot(vec_neg(v), n), 1.0);
	const t_vec	out_per = vec_scale(vec_add(v, vec_scale(n, cos_theta)), eoe);
	const t_vec	out_par = vec_scale(n, -sqrt(fabs(1.0 - vec_mag2(out_per))));
	return (vec_add(out_per, out_par));
}

const t_material_vt
	*dielectric_vt(void)
{
	static const t_material_vt	vt = {
		dielectric_scatter,
		dielectric_destroy
	};

	return (&vt);
}

int
	dielectric_scatter(t_material *mat, t_ray in, t_hit *hit, t_scatter *scatter, t_thread_ctx *ctx)
{
	t_dielectric	*dielectric;
	t_vec			dir;
	FLOAT			cos_theta;
	FLOAT			sin_theta;
	FLOAT			rr;

	dielectric = (t_dielectric *) mat;
	scatter->attenuation = vec(1, 1, 1, 0);
	scatter->scattered.pos = hit->pos;
	if (vec_dot(in.dir, hit->normal) < 0)
		rr = 1.0 / dielectric->ir;
	else
		rr = dielectric->ir;
	cos_theta = fmin(vec_dot(vec_neg(in.dir), hit->local_normal), 1.0);
	sin_theta = sqrt(1.0 - cos_theta * cos_theta);
	if (rr * sin_theta > 1.0 || reflectance(cos_theta, rr) > rt_random_float(&ctx->seed))
		dir = reflect(in.dir, hit->local_normal);
	else
		dir = refract(in.dir, hit->local_normal, rr);
	scatter->scattered.dir = vec_norm(dir);
	return (1);
}

void
	dielectric_destroy(t_material *mat)
{
	rt_free(mat);
}
