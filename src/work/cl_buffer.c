/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   cl_buffer.c                                    #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 12:02:09 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/25 12:02:09 by csteenvo                                 */
/*                                                                            */
/* ************************************************************************** */

#include "cl_impl.h"
#include "util.h"

#if RT_USE_OPENCL

static void
	work_copy_ptrs(t_work *work, t_opencl_ctx *cl_ctx)
{
	t_world	*w;

	w = work->state->world;
	cl_ctx->primitives_mem = work_copy_ptr(cl_ctx,
			w->primitives_size, w->primitives, 6);
	cl_ctx->materials_mem = work_copy_ptr(cl_ctx,
			w->materials_size, w->materials, 7);
	cl_ctx->vertices_mem = work_copy_ptr(cl_ctx,
			w->vertices_size, w->vertices, 8);
	cl_ctx->accel_nodes_mem = work_copy_ptr(cl_ctx,
			w->accel_nodes_size, w->accel_nodes, 9);
	cl_ctx->accel_indices_mem = work_copy_ptr(cl_ctx,
			w->accel_indices_size, w->accel_indices, 10);
	cl_ctx->accel_degenerates_mem = work_copy_ptr(cl_ctx,
			w->accel_degenerates_size, w->accel_degenerates, 11);
	cl_ctx->texture_data_mem = work_copy_ptr(cl_ctx,
			w->texture_data_size, w->texture_data, 12);
	cl_ctx->textures_mem = work_copy_ptr(cl_ctx,
			w->textures_size, w->textures, 13);
	cl_ctx->bxdfs_mem = work_copy_ptr(cl_ctx,
			w->bxdfs_size, w->bxdfs, 14);
	cl_ctx->lights_mem = work_copy_ptr(cl_ctx,
			w->lights_size, w->lights, 15);
	cl_ctx->leaf_data_mem = work_copy_ptr(cl_ctx,
			w->leaf_data_size, w->leaf_data, 16);
}

void
	work_create_buffers(t_work *work, t_opencl_ctx *cl_ctx)
{
	cl_int	status;

	cl_ctx->result_mem[0] = clCreateBuffer(cl_ctx->context, CL_MEM_WRITE_ONLY,
			sizeof(*cl_ctx->result) * RT_WORK_OPENCL_CHUNK_SIZE, NULL, &status);
	rt_assert(status == CL_SUCCESS, "clCreateBuffer result[0] failed");
	cl_ctx->result_mem[1] = clCreateBuffer(cl_ctx->context, CL_MEM_WRITE_ONLY,
			sizeof(*cl_ctx->result) * RT_WORK_OPENCL_CHUNK_SIZE, NULL, &status);
	rt_assert(status == CL_SUCCESS, "clCreateBuffer result[1] failed");
	cl_ctx->indices_mem[0] = clCreateBuffer(cl_ctx->context, CL_MEM_READ_WRITE,
			sizeof(*cl_ctx->indices), NULL, &status);
	rt_assert(status == CL_SUCCESS, "clCreateBuffer indices[0] failed");
	cl_ctx->indices_mem[1] = clCreateBuffer(cl_ctx->context, CL_MEM_READ_WRITE,
			sizeof(*cl_ctx->indices), NULL, &status);
	rt_assert(status == CL_SUCCESS, "clCreateBuffer indices[1] failed");
	cl_ctx->world_mem = work_copy_ptr(cl_ctx,
			offsetof(t_world, primitives), work->state->world, 0);
	cl_ctx->ctx_mem = work_copy_ptr(cl_ctx,
			sizeof(*cl_ctx->ctx) * RT_WORK_OPENCL_GLOBAL_SIZE, cl_ctx->ctx, 1);
	work_copy_ptrs(work, cl_ctx);
}

void
	work_null_buffers(t_opencl_ctx *cl_ctx)
{
	cl_ctx->world_mem = NULL;
	cl_ctx->ctx_mem = NULL;
	cl_ctx->result_mem[0] = NULL;
	cl_ctx->result_mem[1] = NULL;
	cl_ctx->indices_mem[0] = NULL;
	cl_ctx->indices_mem[1] = NULL;
	cl_ctx->primitives_mem = NULL;
	cl_ctx->materials_mem = NULL;
	cl_ctx->vertices_mem = NULL;
	cl_ctx->accel_nodes_mem = NULL;
	cl_ctx->accel_indices_mem = NULL;
	cl_ctx->accel_degenerates_mem = NULL;
	cl_ctx->texture_data_mem = NULL;
	cl_ctx->textures_mem = NULL;
	cl_ctx->bxdfs_mem = NULL;
	cl_ctx->lights_mem = NULL;
	cl_ctx->leaf_data_mem = NULL;
}

void
	work_release_buffers(t_opencl_ctx *cl_ctx)
{
	work_destroy_ptr(cl_ctx->world_mem);
	work_destroy_ptr(cl_ctx->ctx_mem);
	work_destroy_ptr(cl_ctx->result_mem[0]);
	work_destroy_ptr(cl_ctx->result_mem[1]);
	work_destroy_ptr(cl_ctx->indices_mem[0]);
	work_destroy_ptr(cl_ctx->indices_mem[1]);
	work_destroy_ptr(cl_ctx->primitives_mem);
	work_destroy_ptr(cl_ctx->materials_mem);
	work_destroy_ptr(cl_ctx->vertices_mem);
	work_destroy_ptr(cl_ctx->accel_nodes_mem);
	work_destroy_ptr(cl_ctx->accel_indices_mem);
	work_destroy_ptr(cl_ctx->accel_degenerates_mem);
	work_destroy_ptr(cl_ctx->texture_data_mem);
	work_destroy_ptr(cl_ctx->textures_mem);
	work_destroy_ptr(cl_ctx->bxdfs_mem);
	work_destroy_ptr(cl_ctx->lights_mem);
	work_destroy_ptr(cl_ctx->leaf_data_mem);
}

#endif
