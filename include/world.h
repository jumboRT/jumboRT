#ifndef WORLD_H
# define WORLD_H

# define RT_PRIMITIVE_ALIGN 4
# define RT_MATERIAL_ALIGN 4

# define RT_SHAPE_TRIANGLE 0
# define RT_SHAPE_SPHERE 1

/* # define RT_RAY_MIN 0.001 */

# include "rtmath.h"
# include "cl.h"

typedef struct s_image_meta		t_image_meta;
typedef struct s_camera			t_camera;
typedef struct s_world			t_world;
typedef struct s_vertex			t_vertex;
typedef struct s_primitive		t_primitive;
typedef struct s_material		t_material;
typedef struct s_shape_triangle	t_shape_triangle;
typedef struct s_shape_sphere	t_shape_sphere;
typedef struct s_accel_node		t_accel_node;

struct s_image_meta {
	uint64_t	width;
	uint64_t	height;
	uint64_t	samples;
};

struct s_camera {
	t_vec	org;
	t_vec	base;
	t_vec	u;
	t_vec	v;
};

struct s_vertex {
	t_vec		pos;
	t_vec		tangent;
	t_vec		normal;
	t_vec2		uv;
};

struct s_primitive {
	uint16_t	mat_index;
	uint8_t		shape_type;
};

struct s_material {
	uint8_t		mat_type;
};

struct s_shape_triangle {
	t_primitive	base;
	uint32_t	a;
	uint32_t	b;
	uint32_t	c;
};

struct s_shape_sphere {
	t_primitive	base;
	FLOAT		radius;
	t_vec		pos;
};

struct s_accel_node {
	union {
		FLOAT		split;
		uint32_t	one_primitive;
		uint32_t	primitive_ioffset;
	}	a;
	union {
		uint32_t	flags;
		uint32_t	nprims;
		uint32_t	above_child;
	}	b;
};

struct s_world {
	t_image_meta	img_meta;
	t_camera		camera;
	void			*primitives;
	void			*materials;
	t_vertex		*vertices;
	t_accel_node	*accel_nodes;
	uint32_t		*accel_indices;
	uint32_t		primitives_count;
	uint32_t		materials_count;
	uint32_t		vertices_count;
	uint32_t		accel_nodes_count;
	uint32_t		accel_indices_count;
	uint64_t		primitives_size;
	uint64_t		materials_size;
	uint64_t		vertices_size;
	uint64_t		accel_nodes_size;
	uint64_t		accel_indices_size;
	uint64_t		primitives_capacity;
	uint64_t		materials_capacity;
	uint64_t		vertices_capacity;
	uint64_t		accel_nodes_capacity;
	uint64_t		accel_indices_capacity;
};

uint64_t	world_primitive_size(uint8_t shape_type);

int			world_intersect(const GLOBAL t_world *world, t_ray ray, t_hit *hit);
void		world_accel(t_world *world);
void		leaf_create(t_accel_node *leaf, const uint32_t *prim_indices, uint32_t prim_count, uint32_t *out_indices);
void		interior_create(t_accel_node *interior, uint32_t axis, uint32_t above_child, FLOAT offset);

FLOAT		split_pos(t_accel_node node);
uint32_t	nprims(t_accel_node node);
uint32_t	split_axis(t_accel_node node);
uint32_t	is_leaf(t_accel_node node);
uint32_t	above_child(t_accel_node node);

#endif
