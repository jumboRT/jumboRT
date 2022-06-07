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
typedef struct s_triangle		t_triangle;
typedef struct s_sphere			t_sphere;
typedef struct s_cone			t_cone;
typedef struct s_plane			t_plane;
typedef struct s_cylinder		t_cylinder;

typedef struct s_hit			t_hit;
typedef struct s_scene			t_scene;

struct s_tree;

struct s_hit {
	t_vec		pos;
	t_vec		normal;
	t_vec		local_normal;
	t_material	*mat;
	FLOAT		t;
};

struct s_entity_vt {
	int	(*hit)(const t_entity *ent, t_ray ray, t_hit *hit, FLOAT min);
	void (*destroy)(t_entity *ent);
	int (*compare)(t_entity *ent, t_vec pos, t_vec dir);
	int (*get_bounds)(const t_entity *ent, t_vec *a, t_vec *b);
};

struct s_entity {
	const t_entity_vt	*vt;
	size_t				id;
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
	t_material	*mat;
};

struct s_triangle {
	t_entity	base;
	t_vec		pos0;
	t_vec		pos1;
	t_vec		pos2;
	t_vec		normal;
	FLOAT		diameter;
	t_vec		v0;
	t_vec		v1;
	FLOAT		d00;
	FLOAT		d01;
	FLOAT		d11;
	FLOAT		inv_denom;
	t_material	*mat;
};

struct s_sphere {
	t_entity	base;
	t_vec		pos;
	FLOAT		diameter;
	FLOAT		radius;
	t_material	*mat;
};

struct s_cone {
	t_entity	base;
	t_vec		pos;
	t_vec		dir;
	FLOAT		angle;
	FLOAT		radius;
	FLOAT		costheta2;
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
	FLOAT		radius;
	t_material	*mat;
};

struct s_scene {
	t_entity		**entities;
	size_t			count;
	struct s_tree	*tree;
	t_camera		*camera;
	t_ambient_light	*ambient_light;
	t_light			*main_light;
};

const t_entity_vt	*ambient_light_vt(void);
const t_entity_vt	*triangle_vt(void);
const t_entity_vt	*camera_vt(void);
const t_entity_vt	*light_vt(void);
const t_entity_vt	*sphere_vt(void);
const t_entity_vt	*cone_vt(void);
const t_entity_vt	*plane_vt(void);
const t_entity_vt	*cylinder_vt(void);

int					triangle_hit(const t_entity *ent, t_ray ray, t_hit *hit, FLOAT min);
int					sphere_hit(const t_entity *ent, t_ray ray, t_hit *hit, FLOAT min);
int					cone_hit(const t_entity *ent, t_ray ray, t_hit *hit, FLOAT min);
int					plane_hit(const t_entity *ent, t_ray ray, t_hit *hit, FLOAT min);
int					cylinder_hit(const t_entity *ent, t_ray ray, t_hit *hit, FLOAT min);
int					light_hit(const t_entity *ent, t_ray ray, t_hit *hit, FLOAT min);

void				ambient_light_destroy(t_entity *ent);
void				camera_destroy(t_entity *ent);
void				light_destroy(t_entity *ent);
void				triangle_destroy(t_entity *ent);
void				sphere_destroy(t_entity *ent);
void				cone_destroy(t_entity *ent);
void				plane_destroy(t_entity *ent);
void				cylinder_destroy(t_entity *ent);

int					triangle_compare(t_entity *ent, t_vec pos, t_vec dir);
int					sphere_compare(t_entity *ent, t_vec pos, t_vec dir);
int					cone_compare(t_entity *ent, t_vec pos, t_vec dir);
int					plane_compare(t_entity *ent, t_vec pos, t_vec dir);
int					cylinder_compare(t_entity *ent, t_vec pos, t_vec dir);
int					light_compare(t_entity *ent, t_vec pos, t_vec dir);

int					plane_get_bounds(const t_entity *ent, t_vec *a, t_vec *b);
int					triangle_get_bounds(const t_entity *ent, t_vec *a, t_vec *b);
int					sphere_get_bounds(const t_entity *ent, t_vec *a, t_vec *b);
int					cone_get_bounds(const t_entity *ent, t_vec *a, t_vec *b);
int					plane_get_bounds(const t_entity *ent, t_vec *a, t_vec *b);
int					cylinder_get_bounds(const t_entity *ent, t_vec *a, t_vec *b);
int					light_get_bounds(const t_entity *ent, t_vec *a, t_vec *b);

void				scene_destroy(t_scene *scene);

int					sphere_plane_compare(t_vec plane_pos, t_vec plane_dir, t_vec pos, FLOAT radius);
int					box_plane_compare(t_vec plane_pos, t_vec plane_dir, t_vec a, t_vec b);

#endif
