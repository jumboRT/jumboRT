#ifndef PARSER_H
# define PARSER_H

# include "gfx.h"
# include "scene.h"

typedef t_entity				*(*t_parse_proc)(const char **line, char **error);
typedef t_material				*(*t_material_parse_proc)(const char **line, char **error);
typedef struct s_entry			t_entry;
typedef struct s_material_entry	t_material_entry;

struct s_entry {
	const char		*identifier;
	t_parse_proc	proc;
};

struct s_material_entry {
	const char				*identifier;
	t_material_parse_proc	proc;
};

int			rt_scene(t_scene *scene, const char *file, char **error);

t_entity	*rt_ambient_light(const char **line, char **error);
t_entity	*rt_triangle(const char **line, char **error);
t_entity	*rt_camera(const char **line, char **error);
t_entity	*rt_light(const char **line, char **error);
t_entity	*rt_sphere(const char **line, char **error);
t_entity	*rt_plane(const char **line, char **error);
t_entity	*rt_cylinder(const char **line, char **error);
t_entity	*rt_entity(const char **line, char **error);
t_material	*rt_lambertian(const char **line, char **error);
t_material	*rt_metal(const char **line, char **error);
t_material	*rt_dielectric(const char **line, char **error);
t_material	*rt_emitter(const char **line, char **error);
t_material	*rt_material(const char **line, char **error);
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
