/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   world.h                                        #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 12:43:53 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/25 13:37:36 by csteenvo                                 */
/*                                                                            */
/* ************************************************************************** */

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
# include "typedefs.h"

# if ACCEL_USE_ROPES

struct s_intersect_ctx {
	const GLOBAL t_accel_node	*node;
	float						min_t;
	const GLOBAL t_uint32		*prims;
	t_uint32					prim_index;
	t_uint32					prim_count;
};

# else

struct s_stack_node {
	t_uint32	index;
	float		max;
};

struct s_intersect_ctx {
	struct s_stack_node			stack[ACCEL_NODE_STACK_SIZE];
	t_uint32					stack_index;
	const GLOBAL t_accel_node	*node;
	float						min_t;
	float						max_t;
	const GLOBAL t_uint32		*prims;
	t_uint32					prim_index;
	t_uint32					prim_count;
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
	t_int64		mat;
	t_uint32	bxdf;
	float		eta;
	t_int32		next;
	t_int32		prev;
};

struct s_trace_all_ctx {
	GLOBAL t_result		*results;
	GLOBAL unsigned int	*index;
	t_uint64			begin;
	t_uint64			end;
};

struct s_trace_ctx {
	const GLOBAL t_world	*world;
	GLOBAL t_context		*ctx;
	t_ray					ray;
	t_vec					head;
	t_vec					tail;
	const GLOBAL t_material	*volumes[RT_MAX_VOLUMES];
	t_uint32				volume_size;
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
	t_uint64	width;
	t_uint64	height;
	t_uint64	start_x;
	t_uint64	start_y;
	t_uint64	end_x;
	t_uint64	end_y;
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
	t_uint32				flags;
	t_uint32				ambient_mat;
	float					ambient_dist;
	t_uint32				render_mode;
	t_uint64				batch_size;
	t_uint64				trace_batch_size;
	t_uint32				primitives_count;
	t_uint32				materials_count;
	t_uint32				vertices_count;
	t_uint32				accel_nodes_count;
	t_uint32				accel_indices_count;
	t_uint32				accel_degenerates_count;
	t_uint32				textures_count;
	t_uint32				bxdfs_count;
	t_uint32				lights_count;
	t_uint32				leaf_data_count;
	t_uint64				primitives_size;
	t_uint64				materials_size;
	t_uint64				vertices_size;
	t_uint64				accel_nodes_size;
	t_uint64				accel_indices_size;
	t_uint64				accel_degenerates_size;
	t_uint64				textures_size;
	t_uint64				texture_data_size;
	t_uint64				bxdfs_size;
	t_uint64				lights_size;
	t_uint64				leaf_data_size;
	t_uint64				primitives_capacity;
	t_uint64				materials_capacity;
	t_uint64				vertices_capacity;
	t_uint64				accel_nodes_capacity;
	t_uint64				accel_indices_capacity;
	t_uint64				accel_degenerates_capacity;
	t_uint64				textures_capacity;
	t_uint64				texture_data_capacity;
	t_uint64				bxdfs_capacity;
	t_uint64				lights_capacity;
	t_uint64				leaf_data_capacity;
	GLOBAL void				*primitives;
	GLOBAL t_material		*materials;
	GLOBAL t_vertex			*vertices;
	GLOBAL t_accel_node		*accel_nodes;
	GLOBAL t_uint32			*accel_indices;
	GLOBAL t_uint32			*accel_degenerates;
	GLOBAL t_tex			*textures;
	GLOBAL unsigned char	*texture_data;
	GLOBAL t_bxdf_any		*bxdfs;
	GLOBAL t_uint32			*lights;
	GLOBAL t_leaf_data		*leaf_data;
};

t_uint64					project_index(const GLOBAL t_world *world,
								t_uint64 index);
t_ray						project(const GLOBAL t_world *world,
								GLOBAL t_context *ctx, t_uint64 index);

t_uint64					world_primitive_size(t_uint8 shape_type);
t_uint32					prim_mat(const GLOBAL t_primitive *prim);

t_sphere					make_sphere(const GLOBAL t_world *world,
								const GLOBAL t_void *ptr);
t_triangle					make_triangle(const GLOBAL t_world *world,
								const GLOBAL t_void *ptr);

const GLOBAL t_primitive	*get_prim_const(const GLOBAL t_world *world,
								t_uint32 index);
GLOBAL t_primitive			*get_prim(GLOBAL t_world *world,
								t_uint32 index);
const GLOBAL t_material		*get_mat_const(const GLOBAL t_world *world,
								t_uint32 index);
GLOBAL t_material			*get_mat(GLOBAL t_world *world,
								t_uint32 index);
GLOBAL unsigned char		*get_tex_data(GLOBAL t_world *world,
								t_uint32 index);
const GLOBAL unsigned char	*get_tex_data_const(const GLOBAL t_world *world,
								t_uint32 index);
const GLOBAL t_tex			*get_tex_const(const GLOBAL t_world *world,
								t_uint32 index);
const GLOBAL t_bxdf			*get_bxdf_const(const GLOBAL t_world *world,
								t_uint32 index);
GLOBAL t_bxdf				*get_bxdf(GLOBAL t_world *world,
								t_uint32 index);
t_vec						get_vertex(const GLOBAL t_world *world,
								t_uint32 index);
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
								t_uint32 bump_map, t_vec2 uv);
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
								t_uint64 begin, int depth);
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

const GLOBAL t_uint32		*node_prims(const GLOBAL t_world *world,
								const GLOBAL t_accel_node *node);
#endif
