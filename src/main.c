#include "work.h"
#include "util.h"
#include "world_impl.h"
#include "parser.h"

#include <ft_printf.h>
#include <stdlib.h>
#include <unistd.h>


#include <stdio.h>

void
	world_gen(t_world *world)
{
	t_shape_sphere	sphere;
	size_t			i;

	sphere.base.shape_type = RT_SHAPE_SPHERE;
	sphere.base.mat_index = 0;
	sphere.radius = 0.04;
	i = 0;
	while (i < 400)
	{
		sphere.pos = vec(1.0, i % 20 / 10.0 - 1, (size_t) (i / 20) / 10.0 - 1);
		world_add_primitive(world, &sphere, sizeof(sphere));
		i += 1;
	}
	init_camera(world);
}

void
	world_load(t_world *world, const char *filename)
{
	char		*file;
	t_parse_ctx	ctx;
	char		*error;

	file = rt_readfile(filename, &error, NULL);
	if (file == NULL)
	{
		ft_fprintf(STDERR_FILENO, "Error\n%s\n", error);
		rt_free(error);
		exit(EXIT_FAILURE);
	}
	ctx.data = file;
	ctx.filename = filename;
	ctx.line = 1;
	ctx.column = 1;
	rt_world(world, &ctx);
	rt_free(file);
}

#define RT_VERBOSE
void
	dump_tree(t_world *world, uint32_t offset, int depth, t_vec min, t_vec max)
{
	int				i;
	t_accel_node	*node;
	t_vec			vec;

	i = 0;
	while (i < depth)
	{
		fprintf(stderr, "  ");
		i += 1;
	}
	node = &world->accel_nodes[offset];
	if (is_leaf(*node))
	{
#ifdef RT_VERBOSE
		if (nprims(*node) == 1)
			fprintf(stderr, "leaf %d (%f %f %f | %f %f %f), one %d (offset %d)\n", (int) nprims(*node), x(min), y(min), z(min), x(max), y(max), z(max), node->a.one_primitive, offset);
		else
			fprintf(stderr, "leaf %d (%f %f %f | %f %f %f), first %d (offset %d)\n", (int) nprims(*node), x(min), y(min), z(min), x(max), y(max), z(max), world->accel_indices[node->a.primitive_ioffset], offset);
#else
		fprintf(stderr, "leaf %d\n", (int) nprims(*node));
#endif
	}
	else
	{
#ifdef RT_VERBOSE
		fprintf(stderr, "branch (%f %f %f | %f %f %f) (%f %d) (offset %d)\n", x(min), y(min), z(min), x(max), y(max), z(max), split_pos(*node), split_axis(*node), offset);
#else
		fprintf(stderr, "branch\n");
#endif
		vec = vec_set(max, split_axis(*node), split_pos(*node));
		dump_tree(world, offset + 1, depth + 1, min, vec);
		vec = vec_set(min, split_axis(*node), split_pos(*node));
		dump_tree(world, above_child(*node), depth + 1, vec, max);
	}
}

int
	main(int argc, char **argv)
{
	t_image			image;
	t_world			world;
	t_state			state;
	t_work			work;
	size_t			i;

	image.width = 1920;
	image.height = 1080;
	image.data = rt_malloc(sizeof(*image.data) * image.width * image.height);
	i = 0;
	while (i < image.width * image.height)
	{
		image.data[i].samples = 0;
		image.data[i].color = vec(0, 0, 0);
		i += 1;
	}
	state.image = &image;
	state.world = &world;
	world_create(&world);
	world.img_meta.width = image.width;
	world.img_meta.height = image.height;
	world.img_meta.samples = 1;
	if (argc == 1)
		world_gen(&world);
	else
		world_load(&world, argv[1]);
	world_accel(&world);
	printf("%d\n", (int) world.accel_nodes_count);
	dump_tree(&world, 0, 0, vec(-RT_HUGE_VAL, -RT_HUGE_VAL, -RT_HUGE_VAL), vec(RT_HUGE_VAL, RT_HUGE_VAL, RT_HUGE_VAL));
	return (EXIT_SUCCESS);
	work_create(&work, &state);
	work.work_size = world.img_meta.width * world.img_meta.height * world.img_meta.samples;
	work.work_index = 0;
	work.work_progress = 0;
	work_resume(&work);
	while (work.work_progress < work.work_size)
	{
		work_update(&work);
		usleep(1000);
	}
	rt_write_ppm("image.ppm", &image);
	work_destroy(&work);
	return (EXIT_SUCCESS);
}
