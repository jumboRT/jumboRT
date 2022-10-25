/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   impl_add1.c                                    #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 12:02:27 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/25 12:02:27 by csteenvo                                 */
/*                                                                            */
/* ************************************************************************** */

#include "world_impl.h"

#include "mat.h"
#include "util.h"
#include "shape.h"

t_uint32
	world_add_material(t_world *world, t_material *material)
{
	size_t	old_size;

	world->materials_count += 1;
	old_size = world->materials_size;
	world->materials_size += sizeof(*material);
	world->materials = world_reallog(world->materials,
			&world->materials_capacity, world->materials_size);
	rt_memcpy((char *) world->materials + old_size,
		material, sizeof(*material));
	return (old_size / sizeof(*material));
}

t_uint32
	world_add_primitive(t_world *world, void *primitive, size_t size)
{
	size_t				old_size;
	const t_material	*mat;

	world->primitives_count += 1;
	old_size = world->primitives_size;
	world->primitives_size += (size + RT_PRIMITIVE_ALIGN - 1)
		/ RT_PRIMITIVE_ALIGN * RT_PRIMITIVE_ALIGN;
	world->primitives = world_reallog(world->primitives,
			&world->primitives_capacity, world->primitives_size);
	rt_memcpy((char *) world->primitives + old_size, primitive, size);
	mat = get_mat_const(world, prim_mat(primitive));
	if (!prim_is_infinite(primitive) && (mat->flags & RT_MAT_EMITTER))
		world_add_light(world, old_size / RT_PRIMITIVE_ALIGN);
	return (old_size / RT_PRIMITIVE_ALIGN);
}

t_uint32
	world_add_vertex(t_world *world, t_vertex *vertex)
{
	size_t	old_size;

	world->vertices_count += 1;
	old_size = world->vertices_size;
	world->vertices_size += sizeof(*vertex);
	world->vertices = world_reallog(world->vertices,
			&world->vertices_capacity, world->vertices_size);
	rt_memcpy((char *) world->vertices + old_size, vertex, sizeof(*vertex));
	return (old_size / sizeof(*vertex));
}

t_uint32
	world_add_light(t_world *world, t_uint32 light)
{
	size_t	old_size;

	world->lights_count += 1;
	old_size = world->lights_size;
	world->lights_size += sizeof(light);
	world->lights = world_reallog(world->lights,
			&world->lights_capacity, world->lights_size);
	rt_memcpy((char *) world->lights + old_size, &light, sizeof(light));
	return (old_size / sizeof(light));
}
