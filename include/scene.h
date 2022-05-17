#ifndef SCENE_H
# define SCENE_H

# include "gfx.h"
# include "rtmath.h"
# include <stddef.h>

typedef struct s_entity_vt			t_entity_vt;
typedef struct s_entity				t_entity;
typedef struct s_object				t_object;
typedef struct s_ambient_lighting	t_ambient_lighting;
typedef struct s_camera				t_camera;
typedef struct s_light				t_light;
typedef struct s_sphere				t_sphere;
typedef struct s_plane				t_plane;
typedef struct s_cylinder			t_cylinder;
typedef struct s_cone				t_cone;
typedef struct s_hyperboloid		t_hyperboloid;
typedef struct s_paraboloid			t_paraboloid;

typedef struct s_material_vt		t_material_vt;
typedef struct s_material			t_material;
typedef struct s_basic_material		t_basic_material;

typedef struct s_hit				t_hit;
typedef struct s_props				t_props;
typedef struct s_scene				t_scene;

struct s_props {
	t_vec	emittance;
	t_vec	reflectance;
	t_vec	refractance;
	t_vec	normal;
};

struct s_material_vt {
	void	(*props)(t_material *mat, t_vec uv, t_props *props);
};

struct s_material {
	t_material_vt	*vt;
};

struct s_basic_material {
	t_material	base;
	t_vec		color;
};

struct s_hit {
	t_vec	pos;
	t_vec	normal;
	t_vec	uv;
};

struct s_entity_vt {
	int	(*hit)(t_entity *ent, t_vec pos, t_vec dir, t_hit *hit);
};

struct s_entity {
	t_entity_vt	*vt;
};

struct s_object {
	t_entity		base;	
	t_vec			pos;
	t_material		*mat;
};

struct s_ambient_lighting {
	t_entity	base;
	double		ratio;
	t_vec		color;
};

struct s_camera {
	t_entity	base;
	t_vec		pos;
	t_vec		dir;
	double		fov;	
};

struct s_light {
	t_entity	base;
	t_vec		pos;
	double		brightness;
	t_vec		color;
};

struct s_sphere {
	t_object	base;
	double		diameter;
};

struct s_plane {
	t_object	base;
	t_vec		dir;
	t_vec		offset;
	t_vec		size;
};

struct s_cylinder {
	t_object	base;
	t_vec		dir;
	double		diameter;
	double		height;
};

struct s_cone {
	t_object	base;
};

struct s_hyperboloid {
	t_object	base;
};

struct s_paraboloid {
	t_object	base;
};

struct s_scene {
	t_entity			**entities;
	size_t				count;
	t_camera			*camera;
	t_ambient_lighting	*ambient_lighting;
};

#endif
