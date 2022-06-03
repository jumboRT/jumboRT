#ifndef SCENE_H
# define SCENE_H

# include "material.h"
# include "util.h"
# include "rtmath.h"
# include <stddef.h>

typedef struct s_entity_vt		t_entity_vt;
typedef struct s_entity			t_entity;
typedef struct s_ambient_light	t_ambient_light;
typedef struct s_camera			t_camera;
typedef struct s_light			t_light;
typedef struct s_sphere			t_sphere;
typedef struct s_cone			t_cone;
typedef struct s_plane			t_plane;
typedef struct s_cylinder		t_cylinder;

typedef struct s_hit			t_hit;
typedef struct s_scene			t_scene;

struct s_hit {
	t_vec		pos;
	t_vec		normal;
	t_vec		local_normal;
	t_material	*mat;
	FLOAT		t;
};

struct s_entity_vt {
	int	(*hit)(t_entity *ent, t_ray ray, t_hit *hit, FLOAT min);
	void (*destroy)(t_entity *ent);
};

struct s_entity {
	const t_entity_vt	*vt;
};

struct s_ambient_light {
	t_entity	base;
	FLOAT		ratio;
	t_vec		color;
};

struct s_camera {
	t_entity	base;
	t_vec		pos;
	t_vec		dir;
	FLOAT		fov;	
};

struct s_light {
	t_entity	base;
	t_vec		pos;
	FLOAT		brightness;
	t_vec		color;
};

struct s_sphere {
	t_entity	base;
	t_vec		pos;
	FLOAT		diameter;
	t_material	*mat;
};

struct s_cone {
	t_entity	base;
	t_vec		pos;
	t_vec		dir;
	FLOAT		angle;
	FLOAT		height;
	t_material	*mat;
};

struct s_plane {
	t_entity	base;
	t_vec		pos;
	t_vec		dir;
	t_material	*mat;
};

struct s_cylinder {
	t_entity	base;
	t_vec		pos;
	t_vec		dir;
	FLOAT		diameter;
	FLOAT		height;
	t_material	*mat;
};

struct s_scene {
	t_entity		**entities;
	size_t			count;
	t_camera		*camera;
	t_ambient_light	*ambient_light;
	t_light			*main_light;
};

const t_entity_vt	*ambient_light_vt(void);
const t_entity_vt	*camera_vt(void);
const t_entity_vt	*light_vt(void);
const t_entity_vt	*sphere_vt(void);
const t_entity_vt	*cone_vt(void);
const t_entity_vt	*plane_vt(void);
const t_entity_vt	*cylinder_vt(void);

int					sphere_hit(t_entity *ent, t_ray ray, t_hit *hit, FLOAT min);
int					cone_hit(t_entity *ent, t_ray ray, t_hit *hit, FLOAT min);
int					plane_hit(t_entity *ent, t_ray ray, t_hit *hit, FLOAT min);
int					cylinder_hit(t_entity *ent, t_ray ray, t_hit *hit, FLOAT min);

void				ambient_light_destroy(t_entity *ent);
void				camera_destroy(t_entity *ent);
void				light_destroy(t_entity *ent);
void				sphere_destroy(t_entity *ent);
void				cone_destroy(t_entity *ent);
void				plane_destroy(t_entity *ent);
void				cylinder_destroy(t_entity *ent);

void				scene_destroy(t_scene *scene);

#endif
