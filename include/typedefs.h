/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   typedefs.h                                     #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 13:26:31 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/25 13:31:54 by csteenvo            :      ..            */
/*                                                                            */
/* ************************************************************************** */

#ifndef TYPEDEFS_H
# define TYPEDEFS_H

typedef struct s_accel_init_ctx			t_accel_init_ctx;
typedef struct s_accel_node				t_accel_node;
typedef struct s_bounds					t_bounds;
typedef struct s_bsdf					t_bsdf;
typedef struct s_bxdf					t_bxdf;
typedef struct s_bxdf_bphong			t_bxdf_bphong;
typedef struct s_bxdf_cook_torrance		t_bxdf_cook_torrance;
typedef struct s_bxdf_ctx				t_bxdf_ctx;
typedef struct s_bxdf_diffuse			t_bxdf_diffuse;
typedef struct s_bxdf_mf_reflection		t_bxdf_mf_reflection;
typedef struct s_bxdf_oren_nayar		t_bxdf_oren_nayar;
typedef struct s_bxdf_phong				t_bxdf_phong;
typedef struct s_bxdf_reflective		t_bxdf_reflective;
typedef struct s_bxdf_specular			t_bxdf_specular;
typedef struct s_bxdf_transmissive		t_bxdf_transmissive;
typedef struct s_cam_params				t_cam_params;
typedef struct s_camera					t_camera;
typedef struct s_cone					t_cone;
typedef struct s_context				t_context;
typedef struct s_cylinder				t_cylinder;
typedef struct s_cylinder_ctx			t_cylinder_ctx;
typedef struct s_directive				t_directive;
typedef struct s_edge					t_edge;
typedef struct s_entry					t_entry;
typedef struct s_eta_link				t_eta_link;
typedef struct s_filter					t_filter;
typedef struct s_hit					t_hit;
typedef struct s_hyperboloid			t_hyperboloid;
typedef struct s_image					t_image;
typedef struct s_image_meta				t_image_meta;
typedef struct s_leaf_data				t_leaf_data;
typedef struct s_light_hit				t_light_hit;
typedef struct s_material				t_material;
typedef struct s_node_info				t_node_info;
typedef struct s_opencl_callback_ctx	t_opencl_callback_ctx;
typedef struct s_opencl_compile_ctx		t_opencl_compile_ctx;
typedef struct s_opencl_ctx				t_opencl_ctx;
typedef struct s_opencl_program_ctx		t_opencl_program_ctx;
typedef struct s_opencl_start_ctx		t_opencl_start_ctx;
typedef struct s_options				t_options;
typedef struct s_paraboloid				t_paraboloid;
typedef struct s_parse_ctx				t_parse_ctx;
typedef struct s_perf					t_perf;
typedef struct s_pixel					t_pixel;
typedef struct s_plane					t_plane;
typedef struct s_pool					t_pool;
typedef struct s_prim_info				t_prim_info;
typedef struct s_primitive				t_primitive;
typedef struct s_quadratic				t_quadratic;
typedef struct s_queue					t_queue;
typedef struct s_ray					t_ray;
typedef struct s_ray_min				t_ray_min;
typedef struct s_result					t_result;
typedef struct s_result_block			t_result_block;
typedef struct s_rope_data				t_rope_data;
typedef struct s_sample					t_sample;
typedef struct s_shape_cone				t_shape_cone;
typedef struct s_shape_cylinder			t_shape_cylinder;
typedef struct s_shape_hyperboloid		t_shape_hyperboloid;
typedef struct s_shape_paraboloid		t_shape_paraboloid;
typedef struct s_shape_plane			t_shape_plane;
typedef struct s_shape_point			t_shape_point;
typedef struct s_shape_sphere			t_shape_sphere;
typedef struct s_shape_triangle			t_shape_triangle;
typedef struct s_sphere					t_sphere;
typedef struct s_split					t_split;
typedef struct s_state					t_state;
typedef struct s_task					t_task;
typedef struct s_task_item				t_task_item;
typedef struct s_tex					t_tex;
typedef struct s_trace_all_ctx			t_trace_all_ctx;
typedef struct s_trace_ctx				t_trace_ctx;
typedef struct s_tree_edges				t_tree_edges;
typedef struct s_tree_info				t_tree_info;
typedef struct s_triangle				t_triangle;
typedef struct s_vector					t_vector;
typedef struct s_vertex					t_vertex;
typedef struct s_view					t_view;
typedef struct s_win					t_win;
typedef struct s_work					t_work;
typedef struct s_worker					t_worker;
typedef struct s_world					t_world;
typedef struct s_world_hit				t_world_hit;
typedef struct s_zbuf					t_zbuf;
typedef struct s_zchain					t_zchain;
typedef struct s_zlzctx					t_zlzctx;
typedef struct s_zring					t_zring;
typedef struct s_zstate					t_zstate;
typedef struct s_ztable					t_ztable;
typedef struct s_ztoken					t_ztoken;
typedef struct s_ztoken_data			t_ztoken_data;
typedef struct s_ztree					t_ztree;
typedef struct s_zwtree					t_zwtree;
typedef union u_bxdf_any				t_bxdf_any;

#endif
