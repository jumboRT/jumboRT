#ifndef TEX_H
# define TEX_H

# include "rtmath.h"

typedef struct s_texture_vt		t_texture_vt;
typedef struct s_texture		t_texture;
typedef struct s_image_texture	t_image_texture;
typedef struct s_color_texture	t_color_texture;
typedef struct s_tex			t_tex;
typedef struct s_tex_list		t_tex_list;

struct s_texture_vt {
	t_vec	(*sample)(t_texture *tex, t_vec uv);
	void	(*destroy)(t_texture *tex);
};

struct s_texture {
	const t_texture_vt	*vt;
};

struct s_image_texture {
	t_texture	base;
	t_tex		*tex;
};

struct s_color_texture {
	t_texture	base;
	t_vec		color;
};

struct s_tex {
	char	*name;
	t_vec	*pixels;
	size_t	width;
	size_t	height;
};

struct s_tex_list {
	t_tex	**list;
	size_t	count;
};

void				tex_create(t_tex_list *texs);
void				tex_destroy(t_tex_list *texs);
t_tex				*tex_load(t_tex_list *texs, const char *name, char **error);
t_vec				tex_sample(t_tex *tex, t_vec uv);

const t_texture_vt	*image_texture_vt(void);
const t_texture_vt	*color_texture_vt(void);

t_vec				image_texture_sample(t_texture *tex, t_vec uv);
t_vec				color_texture_sample(t_texture *tex, t_vec uv);

void				image_texture_destroy(t_texture *tex);
void				color_texture_destroy(t_texture *tex);

#endif
