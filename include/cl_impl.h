#ifndef CL_IMPL_H
# define CL_IMPL_H

# ifndef RT_USE_OPENCL
#  define RT_USE_OPENCL 1
# endif

# if RT_USE_OPENCL

# include "work.h"

#  if defined RT_LINUX
#   include <CL/cl.h>
#  elif defined RT_WINDOWS
#   include <CL/cl.h>
#  else
#   include <OpenCL/cl.h>
#  endif

#  ifndef RT_WORK_OPENCL_GLOBAL_SIZE
#   if defined RT_MACOS
#    define RT_WORK_OPENCL_GLOBAL_SIZE (1ULL << 10)
#   else
#    define RT_WORK_OPENCL_GLOBAL_SIZE (1ULL << 16)
#   endif
#  endif

#  ifndef RT_WORK_OPENCL_LOCAL_SIZE
#   if defined RT_MACOS
#    define RT_WORK_OPENCL_LOCAL_SIZE (1ULL << 6)
#   else
#    define RT_WORK_OPENCL_LOCAL_SIZE (1ULL << 5)
#   endif
#  endif

#  ifndef RT_WORK_OPENCL_CHUNK_SIZE
#   if defined RT_MACOS
#    define RT_WORK_OPENCL_CHUNK_SIZE (1ULL << 10)
#   else
#    define RT_WORK_OPENCL_CHUNK_SIZE (1ULL << 18)
#   endif
#  endif

#  if defined RT_MACOS
#   define RT_WORK_OPENCL_BUILD_FLAGS "-I include -D RT_OPENCL -D GLOBAL=__global"
#  else
#   define RT_WORK_OPENCL_BUILD_FLAGS "-I include -D RT_OPENCL -D GLOBAL=__global -cl-fast-relaxed-math"
#  endif

typedef struct s_opencl_ctx				t_opencl_ctx;
typedef struct s_opencl_callback_ctx	t_opencl_callback_ctx;
typedef struct s_opencl_start_ctx		t_opencl_start_ctx;
typedef struct s_opencl_program_ctx		t_opencl_program_ctx;
typedef struct s_opencl_compile_ctx		t_opencl_compile_ctx;

typedef void (*t_opencl_func)(void *, cl_platform_id, cl_device_id);

struct s_opencl_ctx {
	cl_context			context;
	cl_command_queue	command_queue[2];
	cl_program			program;
	cl_kernel			work_kernel;
	cl_mem				world_mem;
	cl_mem				ctx_mem;
	cl_mem				result_mem[2];
	cl_mem				indices_mem[2];
	cl_mem				primitives_mem;
	cl_mem				materials_mem;
	cl_mem				vertices_mem;
	cl_mem				accel_nodes_mem;
	cl_mem				accel_indices_mem;
	cl_mem				accel_degenerates_mem;
	cl_mem				texture_data_mem;
	cl_mem				textures_mem;
	cl_mem				bxdfs_mem;
	cl_mem				lights_mem;
	cl_mem				leaf_data_mem;
	t_context			ctx[RT_WORK_OPENCL_GLOBAL_SIZE];
	t_result			result[RT_WORK_OPENCL_CHUNK_SIZE];
	cl_uint				indices[2];
	int					id;
};

struct s_opencl_callback_ctx {
	t_worker		*worker;
	t_opencl_ctx	*cl_ctx;
	int				id;
	cl_event		event;
	uint64_t		begin;
	uint64_t		end;
};

struct s_opencl_start_ctx {
	t_worker				*worker;
	t_opencl_ctx			*cl_ctx;
	cl_event				kernel_event[2];
	t_opencl_callback_ctx	cb_ctx[2];
};

struct s_opencl_program_ctx {
	cl_context		context;
	cl_platform_id	platform;
	cl_device_id	device;
	cl_program		program;
};

struct s_opencl_compile_ctx {
	char		**cache;
	const void	*input;
	const void	*output;
};

char				*device_token(
		cl_platform_id platform, cl_device_id device);
char				*device_file(
		const char *prefix, cl_platform_id platform, cl_device_id device);
void				*work_start(
		void *data);
void				work_callback(
		cl_event event, cl_int event_command_status, void *user_data);

cl_mem				work_copy_ptr(
		t_opencl_ctx *cl_ctx, size_t size, void *ptr, int arg);
void				work_destroy_ptr(
		cl_mem mem);
void				work_create_buffers(
		t_work *work, t_opencl_ctx *cl_ctx);
void				work_null_buffers(
		t_opencl_ctx *cl_ctx);
void				work_release_buffers(
		t_opencl_ctx *cl_ctx);

cl_command_queue	work_create_queue(cl_context context, cl_device_id device,
		cl_command_queue_properties props);
cl_context			work_create_context(cl_platform_id device);
void				work_destroy_queue(cl_command_queue queue);
void				work_destroy_context(cl_context context);

void				opencl_device_default(
		t_opencl_func func, void *data);
void				opencl_device_by_index(
		int pi, int di, t_opencl_func func, void *data);
void				opencl_device_all(
		t_opencl_func func, void *data);

void				opencl_comp_program(
		t_opencl_program_ctx *ctx, const char *string, size_t length);
void				opencl_load_program(
		t_opencl_program_ctx *ctx, const unsigned char *string, size_t length, int build);
void				opencl_link_program(
		t_opencl_program_ctx *ctx, cl_program *programs, size_t count);
unsigned char		*opencl_save_program(
		t_opencl_program_ctx *ctx, size_t *length);
void				opencl_release_program(
		cl_program program);

void				opencl_comp_program_path(
		t_opencl_program_ctx *ctx, const char *path);
void				opencl_load_program_path(
		t_opencl_program_ctx *ctx, const char *path, int build);
void				opencl_link_program_path(
		t_opencl_program_ctx *ctx, const char *const *paths, size_t count);
void				opencl_save_program_path(
		t_opencl_program_ctx *ctx, const char *path);

void				opencl_compile(const char *i, const char *o);
void				opencl_link(const char **i, const char *o);

# endif

#endif