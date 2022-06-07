#ifndef MATERIAL_H
# define MATERIAL_H

# include "rtmath.h"
# include "util.h"
# include "tex.h"

typedef struct s_scatter		t_scatter;
typedef struct s_material_vt	t_material_vt;
typedef struct s_material		t_material;
typedef struct s_lambertian		t_lambertian;
typedef struct s_metal			t_metal;
typedef struct s_dielectric		t_dielectric;
typedef struct s_emitter		t_emitter;

struct s_hit;

struct s_scatter {
	t_vec	attenuation;
	t_vec	emittance;
	t_ray	scattered;
};

struct s_material_vt {
	int	(*scatter)(t_material *mat, t_ray in, struct s_hit *hit, t_scatter *scatter, t_thread_ctx *ctx);
	void (*destroy)(t_material *mat);
};

struct s_material {
	const t_material_vt	*vt;
};

struct s_lambertian {
	t_material	base;
	t_texture	*albedo;
};

struct s_metal {
	t_material	base;
	t_texture	*albedo;
	FLOAT		fuzzy;
};

struct s_dielectric {
	t_material	base;
	FLOAT		ir;
};

struct s_emitter {
	t_material	base;
	FLOAT		brightness;
	t_texture	*emittance;
	t_material	*child;
};

t_vec				reflect(t_vec v, t_vec n);
t_vec				refract(t_vec v, t_vec n, FLOAT eoe);

const t_material_vt	*lambertian_vt(void);
const t_material_vt	*metal_vt(void);
const t_material_vt	*dielectric_vt(void);
const t_material_vt	*emitter_vt(void);

int					lambertian_scatter(t_material *mat, t_ray in, struct s_hit *hit, t_scatter *scatter, t_thread_ctx *ctx);
int					metal_scatter(t_material *mat, t_ray in, struct s_hit *hit, t_scatter *scatter, t_thread_ctx *ctx);
int					dielectric_scatter(t_material *mat, t_ray in, struct s_hit *hit, t_scatter *scatter, t_thread_ctx *ctx);
int					emitter_scatter(t_material *mat, t_ray in, struct s_hit *hit, t_scatter *scatter, t_thread_ctx *ctx);

void				lambertian_destroy(t_material *mat);
void				metal_destroy(t_material *mat);
void				dielectric_destroy(t_material *mat);
void				emitter_destroy(t_material *mat);

#endif
