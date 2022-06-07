#ifndef PARSER_H
# define PARSER_H

# include "gfx.h"
# include "scene.h"

typedef t_entity				*(*t_parse_proc)(t_scene *scene, const char **line, char **error);
typedef t_material				*(*t_material_parse_proc)(t_scene *scene, const char **line, char **error);
typedef t_texture				*(*t_texture_parse_proc)(t_scene *scene, const char **line, char **error);
typedef struct s_entry			t_entry;
typedef struct s_material_entry	t_material_entry;
typedef struct s_texture_entry	t_texture_entry;

struct s_entry {
	const char		*identifier;
	t_parse_proc	proc;
};

struct s_material_entry {
	const char				*identifier;
	t_material_parse_proc	proc;
};

struct s_texture_entry {
	const char				*identifier;
	t_texture_parse_proc	proc;
};

int			rt_scene(t_scene *scene, const char *file, char **error);

t_entity	*rt_ambient_light(t_scene *scene, const char **line, char **error);
t_entity	*rt_triangle(t_scene *scene, const char **line, char **error);
t_entity	*rt_camera(t_scene *scene, const char **line, char **error);
t_entity	*rt_light(t_scene *scene, const char **line, char **error);
t_entity	*rt_sphere(t_scene *scene, const char **line, char **error);
t_entity	*rt_cone(t_scene *scene, const char **line, char **error);
t_entity	*rt_plane(t_scene *scene, const char **line, char **error);
t_entity	*rt_cylinder(t_scene *scene, const char **line, char **error);
t_entity	*rt_entity(t_scene *scene, const char **line, char **error);
t_material	*rt_lambertian(t_scene *scene, const char **line, char **error);
t_material	*rt_metal(t_scene *scene, const char **line, char **error);
t_material	*rt_dielectric(t_scene *scene, const char **line, char **error);
t_material	*rt_emitter(t_scene *scene, const char **line, char **error);
t_material	*rt_material(t_scene *scene, const char **line, char **error);
t_texture	*rt_image_texture(t_scene *scene, const char **line, char **error);
t_texture	*rt_color_texture(t_scene *scene, const char **line, char **error);
t_texture	*rt_texture(t_scene *scene, const char **line, char **error);
int			rt_space(const char *line, char **error);
size_t		rt_wordlen(const char *str);

const char		*rt_norm_vec(const char *line, char **error, t_vec *dst);
const char		*rt_next_word(const char *line);
const char		*rt_end(const char *line, char **error);
const char		*rt_expect_char(const char *line, char **error, char c);
const char		*rt_color(const char *line, char **error, t_vec *dst);
const char		*rt_pos(const char *line, char **error, t_vec *dst);
const char		*rt_uint(const char *line, char **error, unsigned int *dst);
const char		*rt_skip(const char *line, int (*skip_char)(int));
const char		*rt_float(const char *line, char **error, FLOAT *dst);
#endif
