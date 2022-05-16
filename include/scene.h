#ifndef SCENE_H
# define SCENE_H

typedef struct s_scene	t_scene;
typedef struct s_entity	t_entity;
typedef struct s_camera	t_camera;
typedef struct s_light	t_light;
typedef struct s_object	t_object;

struct s_entity {
	t_vec	pos;
};

struct s_camera {
	t_entity	base;
	t_mat		axis;
};

struct s_object {
	t_entity	base;
};

struct s_light {
	t_entity	base;
};

struct s_scene {
	t_object	*objects;
	size_t		object_count;
	t_light		*lights;
	size_t		light_count;
	t_camera	camera;
	t_light		ambient;
};

#endif