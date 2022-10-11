#ifndef BSDF_H
# define BSDF_H

# include "types.h"
# include "tex.h"
# include "hit.h"

# define RT_BXDF_DIFFUSE			0
# define RT_BXDF_REFLECTIVE			1
# define RT_BXDF_TRANSMISSIVE		2
# define RT_BXDF_MF_REFLECTIVE		3
# define RT_BXDF_COOK_TORRANCE		4
# define RT_BXDF_BLINN_PHONG		5
# define RT_BXDF_PHONG				6
# define RT_BXDF_SPECULAR			7
# define RT_BXDF_COUNT				8

typedef struct s_world				t_world;
typedef struct s_context			t_context;
typedef struct s_trace_ctx			t_trace_ctx;

typedef struct s_bsdf				t_bsdf;
typedef struct s_bxdf				t_bxdf;
typedef struct s_bxdf_diffuse		t_bxdf_diffuse;
typedef struct s_bxdf_reflective	t_bxdf_reflective;
typedef struct s_bxdf_transmissive	t_bxdf_transmissive;
typedef struct s_bxdf_mf_reflection	t_bxdf_mf_reflection;
typedef struct s_bxdf_cook_torrance	t_bxdf_cook_torrance;
typedef struct s_bxdf_bphong		t_bxdf_bphong;
typedef struct s_bxdf_phong			t_bxdf_phong;
typedef struct s_bxdf_specular		t_bxdf_specular;
typedef union u_bxdf_any			t_bxdf_any;
typedef struct s_sample				t_sample;

struct s_bxdf {
	uint32_t	type;
	t_filter	tex; // TODO: move to individual bxdfs
	float		weight;
};

struct s_bxdf_diffuse {
	t_bxdf		base;
};

struct s_bxdf_reflective {
	t_bxdf		base;
};

struct s_bxdf_transmissive {
	t_bxdf		base;
};

struct s_bxdf_cook_torrance {
	t_bxdf		base;
	float		roughness;
	float		k;
};

struct s_bxdf_phong {
	t_bxdf		base;
};

struct s_bxdf_bphong {
	t_bxdf		base;
	t_filter	alpha;
	t_filter	spec;
};

struct s_bxdf_specular {
	t_bxdf	base;
};

union u_bxdf_any {
	t_bxdf					base;
	t_bxdf_diffuse			diffuse;
	t_bxdf_reflective		reflective;
	t_bxdf_transmissive		transmissive;
	t_bxdf_phong			phong;
	t_bxdf_bphong			blinn_phong;
	t_bxdf_cook_torrance	cook_torrance;
	t_bxdf_specular			specular;
};

struct s_bsdf {
	uint32_t	begin;
	uint32_t	end;
	float		weight;
};

struct s_sample {
	const GLOBAL t_bxdf	*bxdf;
	t_vec				bsdf;
	t_vec				wo;
	float				pdf;
};

t_sample	sample(t_vec bsdf, t_vec wo, float pdf);
int			same_hemi(t_vec wa, t_vec wb);

float		costheta(t_vec w);

float		f_dielectric(float costhetai, float etai, float etat);

t_sample	diffuse_sample(t_trace_ctx *ctx, const t_world_hit *hit, const GLOBAL t_bxdf_diffuse *bxdf, t_vec wi);
t_vec		diffuse_f(t_trace_ctx *ctx, const t_world_hit *hit, const GLOBAL t_bxdf_diffuse *bxdf, t_vec wi, t_vec wo);
float		diffuse_pdf(t_trace_ctx *ctx, const t_world_hit *hit, const GLOBAL t_bxdf_diffuse *bxdf, t_vec wi, t_vec wo);

t_sample	reflective_sample(t_trace_ctx *ctx, const t_world_hit *hit, const GLOBAL t_bxdf_reflective *bxdf, t_vec wi);
t_vec		reflective_f(t_trace_ctx *ctx, const t_world_hit *hit, const GLOBAL t_bxdf_reflective *bxdf, t_vec wi, t_vec wo);
float		reflective_pdf(t_trace_ctx *ctx, const t_world_hit *hit, const GLOBAL t_bxdf_reflective *bxdf, t_vec wi, t_vec wo);

t_sample	specular_sample(t_trace_ctx *ctx, const t_world_hit *hit, const GLOBAL t_bxdf_specular *bxdf, t_vec wi);
t_vec		specular_f(t_trace_ctx *ctx, const t_world_hit *hit, const GLOBAL t_bxdf_specular *bxdf, t_vec wi, t_vec wo);
float		specular_pdf(t_trace_ctx *ctx, const t_world_hit *hit, const GLOBAL t_bxdf_specular *bxdf, t_vec wi, t_vec wo);

t_sample	transmissive_sample(t_trace_ctx *ctx, const t_world_hit *hit, const GLOBAL t_bxdf_transmissive *bxdf, t_vec wi);
t_vec		transmissive_f(t_trace_ctx *ctx, const t_world_hit *hit, const GLOBAL t_bxdf_transmissive *bxdf, t_vec wi, t_vec wo);
float		transmissive_pdf(t_trace_ctx *ctx, const t_world_hit *hit, const GLOBAL t_bxdf_transmissive *bxdf, t_vec wi, t_vec wo);

t_sample	fresnel_blend_sample(t_trace_ctx *ctx, const t_world_hit *hit, const GLOBAL t_bxdf_transmissive *bxdf, t_vec wi);
t_vec		fresnel_blend_f(t_trace_ctx *ctx, const t_world_hit *hit, const GLOBAL t_bxdf_transmissive *bxdf, t_vec wi, t_vec wo);
float		fresnel_blend_pdf(t_trace_ctx *ctx, const t_world_hit *hit, const GLOBAL t_bxdf_transmissive *bxdf, t_vec wi, t_vec wo);

t_sample	phong_sample(t_trace_ctx *ctx, const t_world_hit *hit, const GLOBAL t_bxdf_phong *bxdf, t_vec wi);
t_vec		phong_f(t_trace_ctx *ctx, const t_world_hit *hit, const GLOBAL t_bxdf_phong *bxdf, t_vec wi, t_vec wo);
float		phong_pdf(t_trace_ctx *ctx, const t_world_hit *hit, const GLOBAL t_bxdf_phong *bxdf, t_vec wi, t_vec wo);

t_sample	bsdf_sample(t_trace_ctx *ctx, const t_world_hit *hit, t_vec wi);
t_vec		bsdf_f(t_trace_ctx *ctx, const t_world_hit *hit, t_vec wi, t_vec wo);
float		bsdf_pdf(t_trace_ctx *ctx, const t_world_hit *hit, t_vec wi, t_vec wo);
int32_t		bxdf_is_perfspec(const GLOBAL t_bxdf *bxdf); // TODO: compute at parse time
#endif
