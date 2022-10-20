#ifndef WORLD_H
# define WORLD_H

# define RT_PRIMITIVE_ALIGN 16

# define RT_RENDER_MODE_DEFAULT				0
# define RT_RENDER_MODE_GEOMETRIC_NORMAL	1
# define RT_RENDER_MODE_SHADING_NORMAL		2
# define RT_RENDER_MODE_UV					3
# define RT_RENDER_MODE_ALBEDO				4
# define RT_RENDER_MODE_COUNT				5

# define RT_WORLD_HAS_AMBIENT		1

# define RT_MAX_DEPTH				16
# define RT_MAX_VOLUMES				2

# define RT_MAX_ETA					8

# define RT_HASH_GEOMETRY			1
# define RT_HASH_MATERIALS			2

# ifndef ACCEL_NODE_STACK_SIZE
#  define ACCEL_NODE_STACK_SIZE 64
# endif

# ifndef RT_TRACE_LIGHT_SAMPLING
#  define RT_TRACE_LIGHT_SAMPLING 1
# endif

# ifndef RT_TRACE_LIGHT_SLOW
#  define RT_TRACE_LIGHT_SLOW 8
# endif

# include "rtmath.h"
# include "types.h"
# include "rand.h"
# include "hit.h"
# include "accel.h"

typedef struct s_accel_node			t_accel_node;
typedef struct s_bsdf				t_bsdf;
typedef struct s_bxdf				t_bxdf;
typedef struct s_material			t_material;
typedef struct s_tex				t_tex;
typedef struct s_primitive			t_primitive;
typedef struct s_leaf_data			t_leaf_data;
typedef union u_bxdf_any			t_bxdf_any;

typedef struct s_context			t_context;
typedef struct s_image_meta			t_image_meta;
typedef struct s_camera				t_camera;
typedef struct s_world				t_world;
typedef struct s_vertex				t_vertex;
typedef struct s_result				t_result;
typedef struct s_trace_ctx			t_trace_ctx;
typedef struct s_trace_all_ctx		t_trace_all_ctx;
typedef struct s_eta_link			t_eta_link;
typedef struct s_cam_params			t_cam_params;

# if ACCEL_USE_ROPES

struct s_intersect_ctx {
	const GLOBAL t_accel_node	*node;
	float						min_t;
	const GLOBAL uint32_t		*prims;
	uint32_t					prim_index;
	uint32_t					prim_count;
};

# else

struct s_stack_node {
	uint32_t	index;
	float		max;
};

struct s_intersect_ctx {
	struct s_stack_node			stack[ACCEL_NODE_STACK_SIZE];
	uint32_t					stack_index;
	const GLOBAL t_accel_node	*node;
	float						min_t;
	float						max_t;
	const GLOBAL uint32_t		*prims;
	uint32_t					prim_index;
	uint32_t					prim_count;
};

# endif

struct s_cam_params {
	t_vec	org;
	t_vec	dir;
	float	fov;
	float	focus;
	float	blur;
};

struct s_eta_link {
	int64_t		mat;
	uint32_t	bxdf;
	float		eta;
	int32_t		next;
	int32_t		prev;
};

struct s_trace_all_ctx {
	GLOBAL t_result		*results;
	GLOBAL unsigned int	*index;
	uint64_t			begin;
	uint64_t			end;
};

struct s_trace_ctx {
	const GLOBAL t_world	*world;
	GLOBAL t_context		*ctx;
	t_ray					ray;
	t_vec					head;
	t_vec					tail;
	const GLOBAL t_material	*volumes[RT_MAX_VOLUMES];
	uint32_t				volume_size;
	float					time;
	int						specref;
	int						alpha_skips;
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

struct s_world {
	t_image_meta			img_meta;
	t_camera				camera;
	uint32_t				flags;
	uint32_t				ambient_mat;
	float					ambient_dist;
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
	uint32_t				lights_count;
	uint32_t				leaf_data_count;
	uint64_t				primitives_size;
	uint64_t				materials_size;
	uint64_t				vertices_size;
	uint64_t				accel_nodes_size;
	uint64_t				accel_indices_size;
	uint64_t				accel_degenerates_size;
	uint64_t				textures_size;
	uint64_t				texture_data_size;
	uint64_t				bxdfs_size;
	uint64_t				lights_size;
	uint64_t				leaf_data_size;
	uint64_t				primitives_capacity;
	uint64_t				materials_capacity;
	uint64_t				vertices_capacity;
	uint64_t				accel_nodes_capacity;
	uint64_t				accel_indices_capacity;
	uint64_t				accel_degenerates_capacity;
	uint64_t				textures_capacity;
	uint64_t				texture_data_capacity;
	uint64_t				bxdfs_capacity;
	uint64_t				lights_capacity;
	uint64_t				leaf_data_capacity;
	GLOBAL void				*primitives;
	GLOBAL t_material		*materials;
	GLOBAL t_vertex			*vertices;
	GLOBAL t_accel_node		*accel_nodes;
	GLOBAL uint32_t			*accel_indices;
	GLOBAL uint32_t			*accel_degenerates;
	GLOBAL t_tex			*textures;
	GLOBAL unsigned char	*texture_data;
	GLOBAL t_bxdf_any		*bxdfs;
	GLOBAL uint32_t			*lights;
	GLOBAL t_leaf_data		*leaf_data;
};

uint64_t					project_index(const GLOBAL t_world *world,
								uint64_t index);
t_ray						project(const GLOBAL t_world *world,
								GLOBAL t_context *ctx, uint64_t index);

uint64_t					world_primitive_size(uint8_t shape_type);
uint32_t					prim_mat(const GLOBAL t_primitive *prim);

t_sphere					make_sphere(const GLOBAL t_world *world,
								const GLOBAL t_void *ptr);
t_triangle					make_triangle(const GLOBAL t_world *world,
								const GLOBAL t_void *ptr);

const GLOBAL t_primitive	*get_prim_const(const GLOBAL t_world *world,
								uint32_t index);
GLOBAL t_primitive			*get_prim(GLOBAL t_world *world,
								uint32_t index);
const GLOBAL t_material		*get_mat_const(const GLOBAL t_world *world,
								uint32_t index);
GLOBAL t_material			*get_mat(GLOBAL t_world *world,
								uint32_t index);
GLOBAL unsigned char		*get_tex_data(GLOBAL t_world *world,
								uint32_t index);
const GLOBAL unsigned char	*get_tex_data_const(const GLOBAL t_world *world,
								uint32_t index);
const GLOBAL t_tex			*get_tex_const(const GLOBAL t_world *world,
								uint32_t index);
const GLOBAL t_bxdf			*get_bxdf_const(const GLOBAL t_world *world,
								uint32_t index);
GLOBAL t_bxdf				*get_bxdf(GLOBAL t_world *world,
								uint32_t index);
t_vec						get_vertex(const t_world *world,
								uint32_t index);
t_vec						get_albedo(const GLOBAL t_world *world,
								const GLOBAL t_material *mat, t_vec2 uv);

t_vec						local_to_world(const t_world_hit *hit, t_vec v);
t_vec						world_to_local(const t_world_hit *hit, t_vec v);

void						eta_init(t_trace_ctx *trace_ctx, float eta);

void						toggle_volume(t_trace_ctx *ctx,
								const t_world_hit *hit, t_vec new_dir);
void						intersect_volume(const t_trace_ctx *ctx,
								t_world_hit *hit);
t_vec						bump(const GLOBAL t_world *world,
								uint32_t bump_map, t_vec2 uv);
void						fix_normals(const GLOBAL t_world *world,
								t_world_hit *hit, t_ray ray);
void						init_normals(t_world_hit *hit, t_ray ray,
								t_vec normal);
int							alpha_test(const t_trace_ctx *ctx,
								t_world_hit *hit);
void						intersect_partial(const t_trace_ctx *ctx,
								t_world_hit *hit, t_ray ray, float max);
void						intersect_info(const t_trace_ctx *ctx,
								t_world_hit *hit, t_ray ray);
void						intersect_full(const t_trace_ctx *ctx,
								t_world_hit *hit, t_ray ray, float max);
void						intersect_slow(const t_trace_ctx *ctx,
								t_world_hit *hit, t_ray ray, float max);
t_vec						le(t_trace_ctx *ctx, const t_world_hit *hit);
void						world_trace_light(t_trace_ctx *ctx,
								const t_world_hit *hit);

int							world_trace_step(t_trace_ctx *ctx);
t_vec						world_trace(const GLOBAL t_world *world,
								GLOBAL t_context *ctx,
								uint64_t begin, int depth);
void						world_trace_all(const GLOBAL t_world *world,
								GLOBAL t_context *ctx,
								const t_trace_all_ctx *ta_ctx);
void						world_intersect_step(const GLOBAL t_world *world,
								t_ray ray, struct s_intersect_ctx *ctx,
								t_world_hit *hit);
void						world_intersect_tree(const GLOBAL t_world *world,
								t_ray ray, t_world_hit *hit);
void						world_intersect(const GLOBAL t_world *world,
								t_ray ray, t_world_hit *hit);

void						camera_set(const t_world *world, t_camera *camera,
								t_cam_params params);

const GLOBAL uint32_t		*node_prims(const GLOBAL t_world *world,
								const GLOBAL t_accel_node *node);
#endif
