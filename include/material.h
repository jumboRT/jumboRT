#ifndef MATERIAL_H
# define MATERIAL_H

# include "rtmath.h"
# include "util.h"

typedef struct s_scatter		t_scatter;
typedef struct s_material_vt	t_material_vt;
typedef struct s_material		t_material;
typedef struct s_lambertian		t_lambertian;
typedef struct s_metal			t_metal;
typedef struct s_dielectric		t_dielectric;

struct s_hit;

struct s_scatter {
	t_vec	attenuation;
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
	t_vec		albedo;
};

struct s_metal {
	t_material	base;
	t_vec		albedo;
	FLOAT		fuzzy;
};

struct s_dielectric {
	t_material	base;
	FLOAT		ir;
};

t_vec				reflect(t_vec v, t_vec n);
t_vec				refract(t_vec v, t_vec n, FLOAT eoe);

const t_material_vt	*lambertian_vt(void);
const t_material_vt	*metal_vt(void);
const t_material_vt	*dielectric_vt(void);

int					lambertian_scatter(t_material *mat, t_ray in, struct s_hit *hit, t_scatter *scatter, t_thread_ctx *ctx);
int					metal_scatter(t_material *mat, t_ray in, struct s_hit *hit, t_scatter *scatter, t_thread_ctx *ctx);
int					dielectric_scatter(t_material *mat, t_ray in, struct s_hit *hit, t_scatter *scatter, t_thread_ctx *ctx);

void				lambertian_destroy(t_material *mat);
void				metal_destroy(t_material *mat);
void				dielectric_destroy(t_material *mat);

#endif
