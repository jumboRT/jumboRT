#include "world_impl.h"
#include "tex.h"
#include "util.h"
#include "mat.h"

#include <libft.h>

uint32_t
	world_add_tex(t_world *world, const t_tex *tex)
{
	size_t	old_size;

	world->textures_count += 1;
	old_size = world->textures_size;
	world->textures_size += sizeof(*tex);
	world->textures = world_reallog(world->textures,
			&world->textures_capacity, world->textures_size);
	rt_memcpy((char *) world->textures + old_size, tex, sizeof(*tex));
	return (old_size / sizeof(*tex));
}

uint32_t
	world_alloc_tex_data(t_world *world, size_t size)
{
	size_t	old_size;

	old_size = world->texture_data_size;
	world->texture_data_size += sizeof(*world->texture_data) * size;
	world->texture_data = world_reallog(world->texture_data,
			&world->texture_data_capacity, world->texture_data_size);
	return (old_size);
}

void
	world_insert_bxdf_volume(t_world *world,
			t_material *material, void *bxdf, size_t size)
{
	uint32_t	i;

	world->bxdfs_size += sizeof(t_bxdf_any);
	world->bxdfs = world_reallog(world->bxdfs,
			&world->bxdfs_capacity, world->bxdfs_size);
	i = material->volume.begin;
	while (i < material->volume.end
		&& ((t_bxdf*) bxdf)->type > world->bxdfs[i].base.type)
		i += 1;
	ft_memmove(&world->bxdfs[i + 1], &world->bxdfs[i],
		(world->bxdfs_count - i) * sizeof(t_bxdf_any));
	ft_memcpy(&world->bxdfs[i], bxdf, size);
	world->bxdfs_count += 1;
	material->volume.end += 1;
	material->volume.weight += ((t_bxdf *) bxdf)->weight;
}

void
	world_insert_bxdf_surface(t_world *world,
			t_material *material, void *bxdf, size_t size)
{
	uint32_t	i;

	world->bxdfs_size += sizeof(t_bxdf_any);
	world->bxdfs = world_reallog(world->bxdfs,
			&world->bxdfs_capacity, world->bxdfs_size);
	i = material->surface.begin;
	while (i < material->surface.end
		&& ((t_bxdf *) bxdf)->type > world->bxdfs[i].base.type)
		i += 1;
	ft_memmove(&world->bxdfs[i + 1], &world->bxdfs[i],
		(world->bxdfs_count - i) * sizeof(t_bxdf_any));
	ft_memcpy(&world->bxdfs[i], bxdf, size);
	world->bxdfs_count += 1;
	material->surface.end += 1;
	material->surface.weight += ((t_bxdf *) bxdf)->weight;
}

void
	world_insert_bxdf(t_world *world,
			t_material *material, void *bxdf, size_t size)
{
	if (material->flags & RT_MAT_HAS_VOLUME)
		world_insert_bxdf_volume(world, material, bxdf, size);
	else
		world_insert_bxdf_surface(world, material, bxdf, size);
}
