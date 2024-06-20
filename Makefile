NAME					:= RT
CL_NAME					:= kernel

BASE_FILES := main.c main_image.c main_key.c main_signal.c main_update.c \
			  main_util.c main_viewer.c main_window.c main_windows.c \
			  main_worker.c option.c option1.c options.c perf.c
ACCEL_FILES := accel_algo.c accel_info.c accel_info_init.c accel_init.c \
			   accel_load.c accel_ropes.c accel_save.c accel_split_axis.c \
			   accel_util1.c accel_util2.c node1.c node2.c
BSDF_FILES := bsdf.c bsdf_forward.c bsdf_sample_int.c bsdf_util.c diffuse.c \
			  oren_nayar.c phong.c polar.c reflective.c specular.c \
			  transmissive.c util.c
GFX_FILES := color.c win.c
MATH_FILES := abs.c atan.c basis.c clamp.c cone.c cone_sample.c cone_util.c \
			  conversion.c cos.c cylinder.c cylinder_sample.c cylinder_util.c \
			  exp.c gamma.c hyperboloid.c infinite_cylinder.c log.c \
			  max.c min.c mod.c paraboloid.c plane.c polynomial.c pow.c \
			  ray_constr.c sin.c sphere.c sphere_sample.c sqrt.c tan.c \
			  tangent.c triangle.c vec2.c vec2_arith.c vec2_arith_fast.c \
			  vec2_fast.c vec2_get.c vec2_get_fast.c vec_abs.c vec_abs_fast.c \
			  vec_arith.c vec_arith_fast.c vec_clamp.c vec_clamp_fast.c \
			  vec_constr.c vec_constr2.c vec_constr_fast.c vec_constr_fast2.c \
			  vec_eq.c vec_geo.c vec_geo_fast.c vec_get.c vec_get_fast.c \
			  vec_rotate.c vec_set.c vec_size.c vec_size_fast.c vec_value.c
MT_FILES := cond.c cond_mt.c mutex.c mutex_mt.c pool.c pool_create_int.c \
			pool_mt.c task.c thread.c thread_mt.c
NET_FILES := client.c client_create.c client_loop.c compression.c connection.c \
			 data.c handler.c handshake.c job_request.c jobs.c new_work.c \
			 pack.c pack_basic.c pack_packet.c packet.c recv.c send.c \
			 send_results.c size.c string.c unpack.c unpack_basic.c \
			 unpack_packet.c
PARSER_FILES := ambient.c boloid.c camera.c color.c comment.c common.c \
				conditional.c cone.c cylinder.c float.c include.c init.c \
				light.c mat1.c mat2.c material.c material_table.c plane.c \
				sphere.c texture.c texture_table.c triangle.c util.c vec.c \
				vertex.c word.c world.c mat3.c
SHAPE_FILES := bounds.c traits.c
TEX_FILES := filter.c init.c sample.c sample_vector.c
UTIL_FILES := aabb.c atof.c freadfile.c hash.c image.c image_bin.c image_pfm.c \
			  image_ppm.c lib.c memory.c poll.c queue.c random1.c random2.c \
			  readfile.c util.c writefile.c
VECTOR_FILES := sort.c swap.c vector.c view.c
WORK_FILES := cl_buffer.c cl_buffer_util.c cl_compile.c cl_device.c cl_fake.c \
			  cl_name.c cl_program.c cl_program_path.c cl_program_save.c \
			  cl_start.c cl_util.c compute.c context.c int.c kernel.c opencl.c \
			  opencl_create.c server.c server_fake.c single.c thread.c \
			  update.c util.c work.c work_create.c cl_cache.c cl_build.c
WORLD_FILES := camera.c common.c common_prim.c common_tex.c conversion.c \
			   hash.c hash_mat.c hash_prim1.c hash_prim2.c hash_util.c impl.c \
			   impl_add1.c impl_add2.c impl_add3.c intersect.c \
			   intersect_prim.c intersect_prim_util.c intersect_ropeless.c \
			   intersect_ropes.c material.c size.c tex_bmp.c trace.c \
			   trace_intersect.c trace_light.c trace_step.c trace_util.c \
			   trace_volume.c
Z_FILES := data.c decode.c deflate1.c deflate2.c encode.c \
		   encode_zwtree_token.c inflate.c lz77.c lz77_util1.c lz77_util2.c \
		   tree.c util.c wtree.c wtree_find.c wtree_util.c zbuf.c zbuf_read.c \
		   zbuf_write.c zring.c ztable.c

PATCH_VERSION			:= 0
MINOR_VERSION			:= 1
MAJOR_VERSION			:= 0

CL_FILE_NAMES			:= \
	util/random1.c \
	util/random2.c \
	util/aabb.c \
	work/kernel.c \
	work/compute.c \
	$(patsubst %.c,math/%.c,$(MATH_FILES)) \
	$(patsubst %.c,bsdf/%.c,$(BSDF_FILES)) \
	$(patsubst %.c,shape/%.c,$(SHAPE_FILES)) \
	$(patsubst %.c,tex/%.c,$(TEX_FILES)) \
	accel/node1.c \
	accel/node2.c \
	world/common.c \
	world/common_prim.c \
	world/common_tex.c \
	world/conversion.c \
	world/intersect.c \
	world/intersect_prim.c \
	world/intersect_prim_util.c \
	world/intersect_ropeless.c \
	world/intersect_ropes.c \
	world/size.c \
	world/trace.c \
	world/trace_intersect.c \
	world/trace_light.c \
	world/trace_step.c \
	world/trace_util.c \
	world/trace_volume.c

ifdef platform
	ifeq ($(platform),linux)
	else ifeq ($(platform),macos)
	else ifeq ($(platform),win)
	else
$(error "invalid platform $(platform)")
	endif
else
	ifeq ($(shell uname -s),Linux)
		platform = linux
	else
		platform = macos
	endif
endif

FILE_NAMES				:= \
	$(patsubst %.c,util/%.c,$(UTIL_FILES)) \
	$(patsubst %.c,vector/%.c,$(VECTOR_FILES)) \
	$(patsubst %.c,mt/%.c,$(MT_FILES)) \
	$(patsubst %.c,work/%.c,$(WORK_FILES)) \
	$(patsubst %.c,math/%.c,$(MATH_FILES)) \
	$(patsubst %.c,world/%.c,$(WORLD_FILES)) \
	$(patsubst %.c,parser/%.c,$(PARSER_FILES)) \
	$(patsubst %.c,net/%.c,$(NET_FILES)) \
	$(patsubst %.c,z/%.c,$(Z_FILES)) \
	$(patsubst %.c,bsdf/%.c,$(BSDF_FILES)) \
	$(patsubst %.c,tex/%.c,$(TEX_FILES)) \
	$(patsubst %.c,shape/%.c,$(SHAPE_FILES)) \
	$(patsubst %.c,accel/%.c,$(ACCEL_FILES)) \
	$(BASE_FILES)

CC						:= clang
LINK_CMD				:= $(CC)
CFLAGS					:= -Wall -Wextra -Wuninitialized -pedantic -DRT_PATCH_VER=$(PATCH_VERSION) \
					   -DRT_MINOR_VER=$(MINOR_VERSION) -DRT_MAJOR_VER=$(MAJOR_VERSION) $(ANALYZER) \
					   -Wno-strict-prototypes
LFLAGS					:= -Wall -Wextra $(ANALYZER)

SRC_DIR					:= src
LIB_DIR					:= dependencies
OBJ_DIR					:= build
DEP_DIR					:= build

LIBFT_DIR				:= $(LIB_DIR)/libft
LIBFT_LIB				:= $(LIBFT_DIR)/libft.a
FT_PRINTF_DIR			:= $(LIB_DIR)/ft_printf
FT_PRINTF_LIB			:= $(FT_PRINTF_DIR)/libftprintf.a
ifeq ($(platform), macos)
	MLX_DIR				:= $(LIB_DIR)/minilibx_macos
else
	MLX_DIR				:= $(LIB_DIR)/minilibx_linux
endif
MLX_LIB					:= $(MLX_DIR)/libmlx.a


INC_DIR					:= include $(LIBFT_DIR) $(FT_PRINTF_DIR)

CFLAGS          		+=
LFLAGS          		+=

BLACK					:="\033[0;30m"
RED						:="\033[0;31m"
GREEN					:="\033[0;32m"
ORANGE					:="\033[0;33m"
BLUE					:="\033[0;34m"
PURPLE					:="\033[0;35m"
CYAN					:="\033[0;36m"
LIGHT_GRAY				:="\033[0;37m"

DARK_GRAY				:="\033[1;30m"
LIGHT_RED				:="\033[1;31m"
LIGHT_GREEN				:="\033[1;32m"
YELLOW					:="\033[1;33m"
LIGHT_BLUE				:="\033[1;34m"
LIGHT_PURPLE			:="\033[1;35m"
LIGHT_CYAN				:="\033[1;36m"
WHITE					:="\033[1;37m"

RESET					:="\033[0m"

COMPILE_COLOR			:= $(GREEN)
LINK_COLOR				:= $(CYAN)
OBJECT_COLOR			:= $(RED)
CLEAN_COLOR				:= $(PURPLE)

ifndef target
	INC_DIR		+= $(MLX_DIR)
	FILE_NAMES += $(patsubst %.c,gfx/%.c,$(GFX_FILES))
	TARGET_LIBS := $(LIBFT_LIB) $(FT_PRINTF_LIB) $(MLX_LIB)
else ifeq ($(target),joinc)
	CFLAGS += -DRT_BONUS=1 -DRT_USE_LIBC -DRT_JOINC
	TARGET_LIBS := $(LIBFT_LIB) $(FT_PRINTF_LIB)
else
$(error "unknown target $(target)")
endif

ifndef config
	config = distr
endif

ifndef san
	san := address
endif 

ifndef error
	CFLAGS	+= -Werror
else ifeq ($(error),1)
	CFLAGS	+= -Werror
endif

ifeq ($(config), debug)
	CFLAGS		+= -DRT_DEBUG=1 -fno-inline -g3 -O0 -DRT_BACKTRACE
	LFLAGS		+= -DRT_DEBUG=1 -fno-inline -g3
	ifeq ($(san), address)
		CFLAGS	+= -fsanitize=address,undefined
		LFLAGS	+= -fsanitize=address,undefined
	else ifeq ($(san), memory)
		CFLAGS	+= -fsanitize=memory,undefined -fsanitize-memory-track-origins
		LFLAGS	+= -fsanitize=memory,undefined -fsanitize-memory-track-origins
	else ifeq ($(san), thread)
		CLFLAGS	+= -fsanitize=thread,undefined
		LFLAGS	+= -fsanitize=thread,undefined
	endif
else ifeq ($(config), release)
	CFLAGS		+= -g3 -gdwarf-4 -O2 -DRT_DEBUG=1
	LFLAGS		+= -g3 -gdwarf-4 -O2 -DRT_DEBUG=1
else ifeq ($(config), profile)
	CFLAGS		+= -g3 -Ofast -pg -flto -march=native
	LFLAGS		+= -g3 -Ofast -pg -flto -march=native
else ifeq ($(config), distr)
	ifeq ($(platform), linux)
		CFLAGS		+= -g3 -gdwarf-4 -Ofast -flto # -march=native
		LFLAGS		+= -g3 -gdwarf-4 -Ofast -flto # -march=native
	else
		CFLAGS		+= -g3 -Ofast
		LFLAGS		+= -g3 -Ofast
	endif
else
$(error "invalid config $(config"))
endif

SOURCES					:= $(patsubst %.c,$(SRC_DIR)/%.c,$(FILE_NAMES))
OBJECTS					:= $(patsubst %.c,$(OBJ_DIR)/%.o,$(FILE_NAMES))
DEPENDS					:= $(patsubst %.c,$(DEP_DIR)/%.d,$(FILE_NAMES))

CL_SOURCES				:= $(patsubst %.c,$(SRC_DIR)/%.c,$(CL_FILE_NAMES))
CL_OBJECTS				:= $(patsubst %.c,$(OBJ_DIR)/%-cl,$(CL_FILE_NAMES))

# all: $(NAME)
all: cpu_only

bonus: CFLAGS += -DRT_BONUS=1 -DRT_USE_LIBC -DRT_MT -DRT_VECTORIZE
bonus: $(NAME) $(CL_NAME)

cpu_only: CFLAGS += -DRT_BONUS=1 -DRT_USE_LIBC -DRT_MT -DRT_VECTORIZE -DRT_USE_OPENCL=0
cpu_only: $(NAME)

mandatory: CFLAGS += -DRT_BONUS=0 -DRT_USE_OPENCL=0
mandatory: $(NAME)

SILENT			:=

ifndef verbose
	SILENT		:= @
endif


ifeq ($(platform), macos)
	FRAMEWORKS	:= -framework OpenGL -framework AppKit -framework OpenCL
	CFLAGS		+= -DRT_MACOS -DclCreateCommandQueueWithProperties=clCreateCommandQueue
	cl_compile_all_at_once := 1
else ifeq ($(platform), linux)
	ifeq ($(target), joinc)
		FRAMEWORKS	:= -lm -lpthread
	else
		FRAMEWORKS	:= -lX11 -lXext -lm -lpthread
	endif
	CFLAGS		+= -DRT_LINUX
else ifeq ($(platform), win)
	FRAMEWORKS	:= /usr/lib/wine/x86_64-windows/opencl.dll -lws2_32 -Wl,-Bstatic -lwinpthread -Wl,-Bdynamic
	CFLAGS		+= -DRT_WINDOWS
	CC			:= x86_64-w64-mingw32-gcc
	LINK_CMD	:= $(CC)
endif

analyze:
		${MAKE} fclean
		${MAKE} ANALYZER="-fanalyzer"

$(NAME): $(OBJECTS) $(TARGET_LIBS)
	@printf $(LINK_COLOR)Linking$(RESET)\ $(OBJECT_COLOR)$(notdir $@)$(RESET)\\n
	$(SILENT)$(LINK_CMD) -o $@ $(OBJECTS) $(TARGET_LIBS) $(FRAMEWORKS) $(LFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(SILENT)mkdir -p $(@D)
	@printf $(COMPILE_COLOR)Compiling$(RESET)\ $(notdir $<)\\n
	$(SILENT)$(CC) $(CFLAGS) -c -o $@ $< -MMD $(patsubst %,-I%,$(INC_DIR))

$(LIBFT_LIB):
	$(SILENT)${MAKE} -C $(LIBFT_DIR) bonus config=$(config) san=$(san) CC=$(CC)

$(FT_PRINTF_LIB):
	$(SILENT)${MAKE} -C $(FT_PRINTF_DIR) all config=$(config) san=$(san) CC=$(CC)

$(MLX_LIB):
	$(SILENT)${MAKE} -C $(MLX_DIR) CFLAGS="-I$(shell pwd)/$(MLX_DIR)" CC=$(CC) 2>/dev/null

ifeq ($(cl_compile_all_at_once), 1)

$(CL_NAME): $(CL_SOURCES) | $(NAME)
	@printf $(LINK_COLOR)Compiling$(RESET)\ $(OBJECT_COLOR)$(notdir $@)$(LINK_COLOR)\ all\ at\ once$(RESET)\\n
	$(SILENT)sh -c "until CUDA_CACHE_DISABLE=1 ./$(NAME) --build $(CL_NAME) $(CL_SOURCES); do sleep 1; done"
	$(SILENT)touch $@

else

$(CL_NAME): $(CL_OBJECTS) | $(NAME)
	@printf $(LINK_COLOR)Linking$(RESET)\ $(OBJECT_COLOR)$(notdir $@)$(RESET)\\n
	$(SILENT)sh -c "until CUDA_CACHE_DISABLE=1 ./$(NAME) -C $(CL_NAME) $(CL_OBJECTS); do sleep 1; done"
	$(SILENT)touch $@

$(OBJ_DIR)/%-cl: $(SRC_DIR)/%.c $(OBJ_DIR)/%.o | $(NAME)
	$(SILENT)mkdir -p $(@D)
	@printf $(COMPILE_COLOR)Compiling$(RESET)\ $(notdir $<)\ \(cl\)\\n
	$(SILENT)sh -c "until CUDA_CACHE_DISABLE=1 ./$(NAME) -c $@ $<; do sleep 1; done"
	$(SILENT)touch $@

endif

clean:
	@printf $(CLEAN_COLOR)Cleaning\ object\ files\ and\ dependencies$(RESET)\\n
	$(SILENT)rm -rf build
	$(SILENT)${MAKE} -C $(LIBFT_DIR) fclean
	$(SILENT)${MAKE} -C $(FT_PRINTF_DIR) fclean
	-$(SILENT)${MAKE} -C $(MLX_DIR) clean

fclean: clean
	@printf $(CLEAN_COLOR)Cleaning\ output\ files$(RESET)\\n
	$(SILENT)rm -f $(NAME)
	$(SILENT)rm -f $(CL_NAME)

re: fclean
	$(MAKE) all

norm:
	norminette include src

-include $(DEPENDS)
.PHONY: all clean fclean re
