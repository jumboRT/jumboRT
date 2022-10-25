/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   vertex.c                                       #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 12:02:07 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/25 12:02:07 by csteenvo                                 */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

#include "world_impl.h"

void
	rt_exec_vertex(t_world *world, t_parse_ctx *ctx)
{
	t_vertex	vertex;

	vertex.pos = rt_vec(ctx);
	vertex.uv = vec2(0.0, 0.0);
	vertex.normal = vec_0();
	ctx->max_index += 1;
	world_add_vertex(world, &vertex);
}

void
	rt_exec_vertex_texture(t_world *world, t_parse_ctx *ctx)
{
	t_vertex	vertex;

	vertex.pos = rt_vec(ctx);
	vertex.uv = rt_vec2(ctx);
	vertex.normal = vec_0();
	ctx->max_index += 1;
	world_add_vertex(world, &vertex);
}

void
	rt_exec_vertex_normal(t_world *world, t_parse_ctx *ctx)
{
	t_vertex	vertex;

	vertex.pos = rt_vec(ctx);
	vertex.uv = vec2(0.0, 0.0);
	vertex.normal = rt_vec(ctx);
	ctx->max_index += 1;
	world_add_vertex(world, &vertex);
}

void
	rt_exec_vertex_texture_normal(t_world *world, t_parse_ctx *ctx)
{
	t_vertex	vertex;

	vertex.pos = rt_vec(ctx);
	vertex.uv = rt_vec2(ctx);
	vertex.normal = rt_vec(ctx);
	ctx->max_index += 1;
	world_add_vertex(world, &vertex);
}
