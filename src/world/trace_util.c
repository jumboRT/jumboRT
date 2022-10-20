#include "world.h"
#include "mat.h"

t_vec
	bump(const GLOBAL t_world *world, uint32_t bump_map, t_vec2 uv)
{
	float	dfdx;
	float	dfdy;

	dfdx = sample_float_offset(world, bump_map, uv, vec2(2, 0)) * 5;
	dfdy = sample_float_offset(world, bump_map, uv, vec2(0, 2)) * 5;
	dfdx -= sample_float_offset(world, bump_map, uv, vec2(0, 0));
	dfdy -= sample_float_offset(world, bump_map, uv, vec2(0, 0));
	return (vec_scale(vec(
				(-dfdx) / (rt_sqrt(dfdx * dfdx + dfdy * dfdy + 1)),
				(-dfdy) / (rt_sqrt(dfdx * dfdx + dfdy * dfdy + 1)),
				1 / (rt_sqrt(dfdx * dfdx + dfdy * dfdy + 1)),
				0.0),
			1.0));
}

void
	fix_normals(const GLOBAL t_world *world, t_world_hit *hit, t_ray ray)
{
	t_vec	tmp;

	(void) ray;
	if (hit->mat->flags & RT_MAT_HAS_NORMAL)
	{
		tmp = sample_vector(world, hit->mat->normal_map, hit->hit.uv);
		tmp = vec_sub(vec_mul(tmp, vec(2, 2, 1, 1)), vec(1, 1, 0, 0));
		hit->hit.shading_normal = local_to_world(hit, tmp);
	}
	if (hit->mat->flags & RT_MAT_HAS_BUMP)
	{
		tmp = bump(world, hit->mat->bump_map, hit->hit.uv);
		hit->hit.shading_normal = local_to_world(hit, tmp);
	}
}

void
	init_normals(t_world_hit *hit, t_ray ray, t_vec normal)
{
	(void) ray;
	hit->hit.geometric_normal = normal;
	hit->hit.shading_normal = normal;
	hit->hit.dpdu = vec_tangent(hit->hit.geometric_normal);
	hit->hit.dpdv = vec_cross(hit->hit.geometric_normal, hit->hit.dpdu);
}

int
	alpha_test(const t_trace_ctx *ctx, t_world_hit *hit)
{
	float	alpha;
	float	sample;

	if (hit->mat == 0 || hit->is_volume)
		return (1);
	if (!(hit->mat->flags & RT_MAT_HAS_ALPHA))
		return (1);
	sample = rt_random_float(&ctx->ctx->seed);
	alpha = w(filter_sample(ctx->world, hit->mat->alpha, hit->hit.uv));
	return (sample < alpha);
}
