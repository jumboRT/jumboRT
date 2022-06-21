#ifndef WORLD_H
# define WORLD_H

# define RT_PRIMITIVE_ALIGN 4
# define RT_MATERIAL_ALIGN 4

# define RT_SHAPE_TRIANGLE 0
# define RT_SHAPE_SPHERE 1

# define RT_RAY_MIN 0.001

# include "rtmath.h"
# include "cl.h"

typedef struct s_image_meta		t_image_meta;
typedef struct s_world			t_world;
typedef struct s_vertex			t_vertex;
typedef struct s_primitive		t_primitive;
typedef struct s_material		t_material;
typedef struct s_shape_triangle	t_shape_triangle;
typedef struct s_shape_sphere	t_shape_sphere;

struct s_image_meta {
	uint64_t	width;
	uint64_t	height;
	uint64_t	samples;
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

struct s_world {
	t_image_meta	img_meta;
	void			*primitives;
	void			*materials;
	t_vertex		*vertices;
	uint64_t		primitives_size;
	uint64_t		materials_size;
	uint64_t		vertices_size;
};

int	world_intersect(const t_world *world, t_ray ray, t_hit *hit);

#endif
