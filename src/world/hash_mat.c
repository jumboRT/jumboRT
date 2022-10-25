/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   hash_mat.c                                     #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 12:02:28 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/25 12:02:28 by csteenvo                                 */
/*                                                                            */
/* ************************************************************************** */

#include "hash.h"
#include "bsdf.h"
#include "mat.h"

t_uint64
	hash_bsdf(const GLOBAL t_bsdf *bsdf,
			const GLOBAL t_world *world, t_seed *seed)
{
	t_uint64	hash;

	(void) world;
	hash = 0;
	hash ^= hash_data(seed, &bsdf->begin, sizeof(bsdf->begin));
	hash ^= hash_data(seed, &bsdf->end, sizeof(bsdf->end));
	hash ^= hash_data(seed, &bsdf->weight, sizeof(bsdf->weight));
	return (hash);
}

t_uint64
	hash_filter(const GLOBAL t_filter *filter,
			const GLOBAL t_world *world, t_seed *seed)
{
	t_uint64	hash;

	(void) world;
	hash = 0;
	hash ^= hash_data(seed, filter->tex, sizeof(filter->tex));
	return (hash);
}

t_uint64
	hash_mat(const GLOBAL t_material *mat,
			const GLOBAL t_world *world, t_seed *seed)
{
	t_uint64	hash;

	hash = 0;
	hash_data(seed, &mat->flags, sizeof(mat->flags));
	hash_bsdf(&mat->surface, world, seed);
	hash_bsdf(&mat->volume, world, seed);
	hash_filter(&mat->alpha, world, seed);
	hash_filter(&mat->emission, world, seed);
	hash_data(seed, &mat->refractive_index, sizeof(mat->refractive_index));
	hash_data(seed, &mat->emission_exp, sizeof(mat->emission_exp));
	hash_data(seed, &mat->brightness, sizeof(mat->brightness));
	hash_data(seed, &mat->normal_map, sizeof(mat->normal_map));
	hash_data(seed, &mat->bump_map, sizeof(mat->bump_map));
	hash_data(seed, &mat->density, sizeof(mat->density));
	return (hash);
}

t_uint64
	hash_bxdf_int(const GLOBAL t_bxdf_any *bxdf,
			const GLOBAL t_world *world, t_seed *seed)
{
	t_uint64	hash;

	hash = 0;
	(void) world;
	if (bxdf->base.type == RT_BXDF_PHONG)
	{
		hash ^= hash_data(seed, &bxdf->phong.roughness,
				sizeof(bxdf->phong.roughness));
	}
	if (bxdf->base.type == RT_BXDF_BLINN_PHONG)
	{
		hash ^= hash_data(seed, &bxdf->blinn_phong.alpha,
				sizeof(bxdf->blinn_phong.alpha));
		hash ^= hash_data(seed, &bxdf->blinn_phong.spec,
				sizeof(bxdf->blinn_phong.spec));
	}
	return (hash);
}

t_uint64
	hash_bxdf(const GLOBAL t_bxdf_any *bxdf,
			const GLOBAL t_world *world, t_seed *seed)
{
	t_uint64	hash;

	hash = hash_salt(seed, bxdf->base.type);
	hash ^= hash_filter(&bxdf->base.tex, world, seed);
	hash ^= hash_data(seed, &bxdf->base.weight, sizeof(bxdf->base.weight));
	hash ^= hash_bxdf_int(bxdf, world, seed);
	if (bxdf->base.type == RT_BXDF_COOK_TORRANCE)
	{
		hash ^= hash_data(seed, &bxdf->cook_torrance.roughness,
				sizeof(bxdf->cook_torrance.roughness));
		hash ^= hash_data(seed, &bxdf->cook_torrance.k,
				sizeof(bxdf->cook_torrance.k));
	}
	if (bxdf->base.type == RT_BXDF_OREN_NAYAR)
	{
		hash ^= hash_data(seed, &bxdf->oren_nayar.alpha,
				sizeof(bxdf->oren_nayar.alpha));
		hash ^= hash_data(seed, &bxdf->oren_nayar.beta,
				sizeof(bxdf->oren_nayar.beta));
	}
	return (hash);
}
