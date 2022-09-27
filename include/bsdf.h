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
# define RT_BXDF_COUNT				6

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
typedef union u_bxdf_any			t_bxdf_any;

struct s_bxdf {
	uint32_t	type;
	t_filter	tex;
	float		weight;
};

struct s_bxdf_diffuse {
	t_bxdf		base;
};

struct s_bxdf_reflective {
	t_bxdf		base;
	float		fuzzy;
};

struct s_bxdf_transmissive {
	t_bxdf		base;
	t_filter	refraction_tex;
	float		eta;
};

struct s_bxdf_cook_torrance {
	t_bxdf		base;
	float		roughness;
	float		eta;
	float		k;
};

struct s_bxdf_bphong {
	t_bxdf		base;
	t_filter	alpha;
	t_filter	spec;
};

union u_bxdf_any {
	t_bxdf					base;
	t_bxdf_diffuse			diffuse;
	t_bxdf_reflective		reflective;
	t_bxdf_transmissive		transmissive;
	t_bxdf_bphong			blinn_phong;
	t_bxdf_cook_torrance	cook_torrance;
};

struct s_bsdf {
	uint32_t	begin;
	uint32_t	end;
	float		weight;
};

t_vec	f_bsdf_sample(t_trace_ctx *ctx, t_world_hit *hit, t_vec wi, t_vec *wo);
t_vec	f_bsdf_f(t_trace_ctx *ctx, t_world_hit *hit, t_vec wi, t_vec *wo);
#endif
