#ifndef WORLD_H
# define WORLD_H

# define RT_PRIMITIVE_ALIGN 16

# define RT_RENDER_MODE_DEFAULT				0
# define RT_RENDER_MODE_GEOMETRIC_NORMAL	1
# define RT_RENDER_MODE_SHADING_NORMAL		2
# define RT_RENDER_MODE_UV					3

# define RT_SHAPE_TRIANGLE			0
# define RT_SHAPE_SPHERE			1
# define RT_SHAPE_PLANE				2
# define RT_SHAPE_CYLINDER			3
# define RT_SHAPE_CONE				4
# define RT_SHAPE_PARABOLOID		5
# define RT_SHAPE_HYPERBOLOID		6

# define RT_TEX_COLOR				0
# define RT_TEX_TEXTURE				1
# define RT_TEX_CHECKER				2

# define RT_BXDF_DIFFUSE			0
# define RT_BXDF_REFLECTIVE			1
# define RT_BXDF_TRANSMISSIVE		2
# define RT_BXDF_MF_REFLECTIVE		3
# define RT_BXDF_COOK_TORRANCE		4
# define RT_BXDF_COUNT				5

# define RT_MAT_SMOOTH				1
# define RT_MAT_EMITTER				2
# define RT_MAT_HAS_ALPHA			4
# define RT_MAT_HAS_VOLUME			8
# define RT_MAT_HAS_NORMAL			16
# define RT_MAT_HAS_BUMP			32

# define RT_WORLD_HAS_AMBIENT		1

# define RT_MAX_DEPTH				8
# define RT_MAX_VOLUMES				1

# define RT_MAX_ETA					8

# define RT_HASH_GEOMETRY			1
# define RT_HASH_MATERIALS			2

/* # define RT_RAY_MIN 0.001 */

# include "rtmath.h"
# include "cl.h"
# include "aabb.h"

typedef struct s_context			t_context;
typedef struct s_image_meta			t_image_meta;
typedef struct s_camera				t_camera;
typedef struct s_world				t_world;
typedef struct s_vertex				t_vertex;
typedef struct s_primitive			t_primitive;
typedef struct s_material			t_material;
typedef struct s_shape_triangle		t_shape_triangle;
typedef struct s_shape_sphere		t_shape_sphere;
typedef struct s_shape_plane		t_shape_plane;
typedef struct s_shape_cylinder		t_shape_cylinder;
typedef struct s_shape_cone			t_shape_cone;
typedef struct s_shape_paraboloid	t_shape_paraboloid;
typedef struct s_shape_hyperboloid	t_shape_hyperboloid;
typedef struct s_accel_node			t_accel_node;
typedef struct s_world_hit			t_world_hit;
typedef struct s_tex				t_tex;
typedef struct s_bsdf				t_bsdf;
typedef struct s_bxdf				t_bxdf;
typedef struct s_bxdf_diffuse		t_bxdf_diffuse;
typedef struct s_bxdf_reflective	t_bxdf_reflective;
typedef struct s_bxdf_transmissive	t_bxdf_transmissive;
typedef struct s_bxdf_mf_reflection	t_bxdf_mf_reflection;
typedef struct s_bxdf_cook_torrance	t_bxdf_cook_torrance;
typedef union u_bxdf_any			t_bxdf_any;
typedef struct s_result				t_result;
typedef struct s_trace_ctx			t_trace_ctx;
typedef struct s_filter				t_filter;
typedef struct s_eta_link			t_eta_link;

struct s_eta_link {
	int64_t		mat;
	uint32_t	bxdf;
	float		eta;
	int32_t		next;
	int32_t		prev;
};

struct s_trace_ctx {
	t_ray					ray;
	t_vec					head;
	t_vec					tail;
	const GLOBAL t_material	*volumes[RT_MAX_VOLUMES];
	t_eta_link				etas[RT_MAX_ETA];
	uint32_t				volume_size;
};

struct s_result {
	t_vec		color;
};

struct s_context {
	t_seed		seed;
};

struct s_image_meta {
	uint64_t	width;
	uint64_t	height;
};

struct s_camera {
	t_vec	org;
	t_vec	base;
	t_vec	dir;
	t_vec	u;
	t_vec	v;
	t_vec	u_norm;
	t_vec	v_norm;
	float	fov;
	float	focus;
	float	blur;
};

struct s_vertex {
	t_vec		pos;
	t_vec		tangent;
	t_vec		normal;
	t_vec2		uv;
};

struct s_primitive {
	uint32_t	data;
};

struct s_tex {
	uint32_t	type;
	union {
		t_vec	color;
		struct {
			uint64_t	width;
			uint64_t	height;
			uint32_t	offset;
		}	tex;
		struct {
			t_vec odd_color;
			t_vec even_color;
		}	checker;
	}	a;
};

struct s_filter {
	uint32_t	tex[2];
};

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

struct s_bxdf_mf_reflective {
	t_bxdf		base;
	float		alphax;
	float		alphay;
};

struct s_bxdf_cook_torrance {
	t_bxdf		base;
	float		roughness;
	float		eta;
	float		k;
};

union u_bxdf_any {
	t_bxdf					base;
	t_bxdf_diffuse			diffuse;
	t_bxdf_reflective		reflective;
	t_bxdf_transmissive		transmissive;
	t_bxdf_cook_torrance	cook_torrance;
};

struct s_bsdf {
	uint32_t	begin;
	uint32_t	end;
	float		weight;
};

struct s_material {
	uint32_t	flags;
	t_bsdf		surface;
	t_bsdf		volume;
	t_filter	alpha;
	t_filter	emission;
	float		emission_exp;
	float		brightness;
	uint32_t	normal_map;
	uint32_t	bump_map;
	float		density;
};

struct s_shape_triangle {
	t_primitive	base;
	uint32_t	a;
	uint32_t	b;
	uint32_t	c;
};

struct s_shape_sphere {
	t_primitive	base;
	float		radius;
	t_vec		pos;
};

struct s_shape_plane {
	t_primitive	base;
	t_plane		plane;
};

struct s_shape_cylinder {
	t_primitive	base;
	t_cylinder	cylinder;
};

struct s_shape_cone {
	t_primitive	base;
	t_cone		cone;
};

struct s_shape_paraboloid {
	t_primitive		base;
	t_paraboloid	paraboloid;
};

struct s_shape_hyperboloid {
	t_primitive		base;
	t_hyperboloid	hyperboloid;
};

struct s_accel_node {
	union {
		float		split;
		uint32_t	one_primitive;
		uint32_t	primitive_ioffset;
	}	a;
	union {
		uint32_t	flags;
		uint32_t	nprims;
		uint32_t	above_child;
	}	b;
};

struct s_world_hit {
	t_hit						hit;
	t_vec						rel_geometric_normal;
	t_vec						rel_shading_normal;
	const GLOBAL t_primitive	*prim;
};

struct s_world {
	t_image_meta			img_meta;
	t_camera				camera;
	uint32_t				flags;
	uint32_t				ambient_mat;
	uint32_t				render_mode;
	uint64_t				batch_size;
	uint64_t				trace_batch_size;
	uint32_t				primitives_count;
	uint32_t				materials_count;
	uint32_t				vertices_count;
	uint32_t				accel_nodes_count;
	uint32_t				accel_indices_count;
	uint32_t				accel_degenerates_count;
	uint32_t				textures_count;
	uint32_t				bxdfs_count;
	uint64_t				primitives_size;
	uint64_t				materials_size;
	uint64_t				vertices_size;
	uint64_t				accel_nodes_size;
	uint64_t				accel_indices_size;
	uint64_t				accel_degenerates_size;
	uint64_t				textures_size;
	uint64_t				texture_data_size;
	uint64_t				bxdfs_size;
	uint64_t				primitives_capacity;
	uint64_t				materials_capacity;
	uint64_t				vertices_capacity;
	uint64_t				accel_nodes_capacity;
	uint64_t				accel_indices_capacity;
	uint64_t				accel_degenerates_capacity;
	uint64_t				textures_capacity;
	uint64_t				texture_data_capacity;
	uint64_t				bxdfs_capacity;
	GLOBAL void				*primitives;
	GLOBAL t_material		*materials;
	GLOBAL t_vertex			*vertices;
	GLOBAL t_accel_node		*accel_nodes;
	GLOBAL uint32_t			*accel_indices;
	GLOBAL uint32_t			*accel_degenerates;
	GLOBAL t_tex			*textures;
	GLOBAL unsigned char	*texture_data;
	GLOBAL t_bxdf_any		*bxdfs;
};

uint64_t	project_index(const GLOBAL t_world *world, uint64_t index);
t_ray		project(const GLOBAL t_world *world, GLOBAL t_context *ctx, uint64_t index);

uint64_t	world_primitive_size(uint8_t shape_type);

t_vec	f_bsdf_sample(const GLOBAL t_world *world, GLOBAL t_context *ctx, t_trace_ctx *trace_ctx, t_bsdf bsdf, t_world_hit hit, t_vec wiw, t_vec *wow);

uint32_t					prim_type(const GLOBAL t_primitive *prim);
uint32_t					prim_mat(const GLOBAL t_primitive *prim);
const GLOBAL t_primitive	*get_prim_const(const GLOBAL t_world *world, uint32_t index);
GLOBAL t_primitive			*get_prim(GLOBAL t_world *world, uint32_t index);
const GLOBAL t_material		*get_mat_const(const GLOBAL t_world *world, uint32_t index);
GLOBAL t_material			*get_mat(GLOBAL t_world *world, uint32_t index);
GLOBAL unsigned char		*get_tex_data(GLOBAL t_world *world, uint32_t index);
const GLOBAL unsigned char	*get_tex_data_const(const GLOBAL t_world *world, uint32_t index);
const GLOBAL t_tex			*get_tex_const(const GLOBAL t_world *world, uint32_t index);
const GLOBAL t_bxdf			*get_bxdf_const(const GLOBAL t_world *world, uint32_t index);
GLOBAL t_bxdf				*get_bxdf(GLOBAL t_world *world, uint32_t index);
t_vec						get_vertex(const t_world *world, uint32_t index);
t_vec						get_albedo(const GLOBAL t_world *world, const GLOBAL t_material *mat, t_vec2 uv);
t_vec						local_to_world(t_world_hit hit, t_vec v);
t_vec						world_to_local(t_world_hit hit, t_vec v);
void eta_init(t_trace_ctx *trace_ctx, float eta);

t_bounds	prim_bounds(const GLOBAL t_primitive *prim, const GLOBAL t_world *world);
int			prim_intersect(const GLOBAL t_primitive *prim, const GLOBAL t_world *world, t_ray ray, float min, t_world_hit *hit);
void		prim_hit_info(const GLOBAL t_primitive *prim, const GLOBAL t_world *world, t_ray ray, t_world_hit *hit);
int			prim_is_infinite(const GLOBAL t_primitive *prim);
t_vec		filter_sample_offset(const GLOBAL t_world *world, t_filter filter, t_vec2 uv, t_vec2 poffset);
t_vec		filter_sample(const GLOBAL t_world *world, t_filter filter, t_vec2 uv);
t_vec		sample_vector_offset(const GLOBAL t_world *world, uint32_t id, t_vec2 uv, t_vec2 offset);
t_vec		sample_color_offset(const GLOBAL t_world *world, uint32_t id, t_vec2 uv, t_vec2 offset);
float		sample_float_offset(const GLOBAL t_world *world, uint32_t id, t_vec2 uv, t_vec2 offset);
t_vec		sample_vector(const GLOBAL t_world *world, uint32_t id, t_vec2 uv);
t_vec		sample_color(const GLOBAL t_world *world, uint32_t id, t_vec2 uv);
float		sample_float(const GLOBAL t_world *world, uint32_t id, t_vec2 uv);

t_vec		world_trace(const GLOBAL t_world *world, GLOBAL t_context *ctx, t_ray ray, int depth);
void		world_trace_all(const GLOBAL t_world *world, GLOBAL t_context *ctx, GLOBAL t_result *results, uint64_t index, uint64_t begin, uint64_t end, uint64_t stride);
int			world_intersect(const GLOBAL t_world *world, t_ray ray, t_world_hit *hit);
void		world_accel(t_world *world);
void		leaf_create(t_accel_node *leaf, const uint32_t *prim_indices, uint32_t prim_count, uint32_t *out_indices);
void		interior_create(t_accel_node *interior, uint32_t axis, uint32_t above_child, float offset);

void		camera_set(const t_world *world, t_camera *camera, t_vec org, t_vec dir, float fov, float focus, float blur);

float		split_pos(t_accel_node node);
uint32_t	nprims(t_accel_node node);
uint32_t	split_axis(t_accel_node node);
uint32_t	is_leaf(t_accel_node node);
uint32_t	above_child(t_accel_node node);

uint64_t	hash_world(const GLOBAL t_world *world, int flags);

#endif

