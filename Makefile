NAME					:= miniRT
CL_NAME					:= kernel

UTIL_FILES				:= util.c readfile.c freadfile.c memory.c image_bin.c \
						   writefile.c lib.c atof.c random.c queue.c aabb.c \
						   hash.c
VECTOR_FILES			:= vector.c sort.c swap.c view.c
MT_FILES				:= cond.c cond_mt.c mutex.c mutex_mt.c thread.c \
						   thread_mt.c pool.c pool_mt.c
WORK_FILES				:= work.c util.c single.c compute.c thread.c opencl.c \
						   context.c int.c server.c update.c cl_buffer.c \
						   cl_buffer_util.c cl_device.c cl_fake.c cl_name.c \
						   cl_program.c cl_program_path.c cl_program_save.c \
						   cl_start.c cl_util.c cl_compile.c
MATH_FILES				:= plane.c polynomial.c ray_constr.c vec_arith.c \
						   vec_constr.c vec_geo.c vec_get.c vec_size.c sqrt.c \
						   sin.c cos.c tan.c basis.c vec_arith_fast.c \
						   vec_constr_fast.c vec_geo_fast.c vec_get_fast.c \
						   vec_size_fast.c sphere.c triangle.c vec_clamp.c \
						   vec_clamp_fast.c min.c max.c abs.c vec_set.c \
						   pow.c cylinder.c vec_rotate.c cone.c vec2.c \
						   vec2_fast.c mod.c vec2_arith_fast.c exp.c clamp.c \
						   log.c tangent.c vec_abs_fast.c vec_abs.c vec2_arith.c \
						   gamma.c vec_value.c paraboloid.c hyperboloid.c coord.c \
						   conversion.c
WORLD_FILES				:= impl.c intersect.c intersect_prim.c \
						   size.c common.c trace.c camera.c \
						   material.c tex_ppm.c conversion.c hash.c 
PARSER_FILES			:= common.c util.c camera.c vertex.c triangle.c \
						   sphere.c plane.c cylinder.c cone.c comment.c \
						   world.c light.c material.c material_table.c \
						   texture.c init.c ambient.c conditional.c include.c \
						   boloid.c
NET_FILES				:= client.c connection.c pack.c packet.c data.c \
						   handler.c jobs.c string.c size.c compression.c
Z_FILES					:= inflate.c deflate.c zbuf.c data.c tree.c \
						   lz77_new.c encode.c decode.c wtree.c \
						   encode_zwtree_token.c util.c
GFX_FILES				:= win.c
BSDF_FILES				:= bsdf_new.c diffuse.c reflective.c transmissive.c \
						   specular.c util.c phong.c oren_nayar.c
TEX_FILES				:= sample.c filter.c init.c
SHAPE_FILES				:= traits.c bounds.c
ACCEL_FILES				:= accel_algo.c accel_info.c accel_util.c accel_ropes.c \
						   node.c
BASE_FILES				:= main.c options.c perf.c

PATCH_VERSION			:= 0
MINOR_VERSION			:= 1
MAJOR_VERSION			:= 0

CL_FILE_NAMES			:= \
	util/random.c \
	work/compute.c \
	math/plane.c \
	math/polynomial.c \
	math/sphere.c \
	math/triangle.c \
	math/cylinder.c \
	math/cone.c \
	math/ray_constr.c \
	math/vec_arith.c \
	math/vec_arith_fast.c \
	math/vec_constr.c \
	math/vec_constr_fast.c \
	math/vec_geo.c \
	math/vec_geo_fast.c \
	math/vec_get.c \
	math/vec_get_fast.c \
	math/vec_size.c \
	math/vec_size_fast.c \
	math/vec2.c \
	math/vec2_fast.c \
	math/vec2_arith_fast.c \
	math/vec2_arith.c \
	math/vec_rotate.c \
	math/vec_set.c \
	math/vec_abs_fast.c \
	math/vec_abs.c \
	math/tangent.c \
	math/clamp.c \
	math/basis.c \
	math/sqrt.c \
	math/coord.c \
	math/sin.c \
	math/cos.c \
	math/tan.c \
	math/min.c \
	math/max.c \
	math/abs.c \
	math/pow.c \
	math/mod.c \
	math/exp.c \
	math/log.c \
	math/gamma.c \
	math/paraboloid.c \
	math/hyperboloid.c \
	math/conversion.c \
	world/intersect.c \
	world/intersect_prim.c \
	world/size.c \
	world/common.c \
	world/trace.c \
	world/conversion.c \
	bsdf/bsdf_new.c \
	bsdf/diffuse.c \
	bsdf/reflective.c \
	bsdf/specular.c \
	bsdf/transmissive.c \
	bsdf/util.c \
	bsdf/phong.c \
	bsdf/oren_nayar.c \
	tex/sample.c \
	tex/filter.c \
	shape/traits.c \
	shape/bounds.c \
	accel/node.c

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
					   -DRT_MINOR_VER=$(MINOR_VERSION) -DRT_MAJOR_VER=$(MAJOR_VERSION) $(ANALYZER)
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

CFLAGS          		+= -DRT_MT
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
	CFLAGS += -DRT_BONUS -DRT_USE_LIBC -DRT_JOINC
	TARGET_LIBS := $(LIBFT_LIB) $(FT_PRINTF_LIB)
else
$(error "unknown target $(target)")
endif

ifdef use_opencl
	ifeq ($(use_opencl),0)
		CFLAGS += -DRT_USE_OPENCL=0
	endif
endif

ifndef config
	config = distr
endif

ifndef san
	san := address
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
	CFLAGS		+= -g3 -O2 -DRT_DEBUG=1
	LFLAGS		+= -g3 -O2 -DRT_DEBUG=1
else ifeq ($(config), profile)
	CFLAGS		+= -g3 -Ofast -pg -flto -march=native
	LFLAGS		+= -g3 -Ofast -pg -flto -march=native
else ifeq ($(config), distr)
	ifeq ($(platform), linux)
		CFLAGS		+= -g3 -Ofast -flto -march=native
		LFLAGS		+= -g3 -Ofast -flto -march=native
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
all: bonus #TODO CHANGE THIS BEFORE TURNING IN!

bonus: CFLAGS += -DRT_BONUS -DRT_USE_LIBC
bonus: $(NAME) $(CL_NAME)

mandatory: $(NAME)

SILENT			:=

ifndef verbose
	SILENT		:= @
endif


ifeq ($(platform), macos)
	FRAMEWORKS	:= -framework OpenGL -framework AppKit -framework OpenCL
	CFLAGS		+= -DRT_MACOS -DclCreateCommandQueueWithProperties=clCreateCommandQueue
	COMPILER_CFLAGS	+= -DRT_MACOS
	COMPILER_EXE := compile
else ifeq ($(platform), linux)
	ifeq ($(target), joinc)
		FRAMEWORKS	:= -lm -lpthread -lOpenCL
	else
		FRAMEWORKS	:= -lX11 -lXext -lm -lOpenCL -lpthread
	endif
	CFLAGS		+= -DRT_LINUX
	COMPILER_CFLAGS	+= -DRT_LINUX
	COMPILER_EXE := compile
else ifeq ($(platform), win)
	FRAMEWORKS	:= /usr/lib/wine/x86_64-windows/opencl.dll -lws2_32 -Wl,-Bstatic -lwinpthread -Wl,-Bdynamic
	CFLAGS		+= -DRT_WINDOWS
	COMPILER_CFLAGS	+= -DRT_WINDOWS
	COMPILER_EXE := compile.exe
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
	$(SILENT)${MAKE} -C $(MLX_DIR) CFLAGS="$(CFLAGS) -I$(shell pwd)/$(MLX_DIR)" CC=$(CC)

$(CL_NAME): $(CL_OBJECTS) | $(NAME)
	@printf $(LINK_COLOR)Linking$(RESET)\ $(OBJECT_COLOR)$(notdir $@)$(RESET)\\n
	$(SILENT)sh -c "until CUDA_CACHE_DISABLE=1 ./miniRT -C $(CL_NAME) $(CL_OBJECTS); do sleep 1; done"
	$(SILENT)touch $@

$(OBJ_DIR)/%-cl: $(SRC_DIR)/%.c | $(NAME)
	$(SILENT)mkdir -p $(@D)
	@printf $(COMPILE_COLOR)Compiling$(RESET)\ $(notdir $<)\ \(cl\)\\n
	$(SILENT)sh -c "until CUDA_CACHE_DISABLE=1 ./miniRT -c $@ $<; do sleep 1; done"
	$(SILENT)touch $@

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

-include $(DEPENDS)
.PHONY: all clean fclean re
