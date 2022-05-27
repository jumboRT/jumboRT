NAME					:= miniRT

MATH_FILES				:= common.c init.c scalar.c vec.c debug.c mul.c float.c color.c
GFX_FILES				:= win.c img.c draw.c hook.c
MT_FILES				:= mutex.c thread.c mutex_mt.c thread_mt.c cond.c cond_mt.c
SCENE_FILES				:= sphere.c plane.c light.c cylinder.c camera.c ambient_light.c
MATERIAL_FILES			:= lambertian.c metal.c dielectric.c
BASE_FILES				:= main.c events.c threads.c render_util.c projection.c trace.c
PARSER_FILES			:= common.c light.c parser.c camera.c object.c material.c
UTIL_FILES				:= atof.c memdup.c readfile.c random.c util.c

ifndef platform
	ifeq ($(shell uname -s),Linux)
		platform = linux
	else
		platform = macos
	endif
endif

FILE_NAMES				:= \
	$(patsubst %,util/%,$(UTIL_FILES)) \
	$(patsubst %,math/%,$(MATH_FILES)) \
	$(patsubst %,gfx/%,$(GFX_FILES)) \
	$(patsubst %,mt/%,$(MT_FILES)) \
	$(patsubst %,scene/%,$(SCENE_FILES)) \
	$(patsubst %,material/%,$(MATERIAL_FILES)) \
	$(patsubst %,parser/%,$(PARSER_FILES)) \
	$(BASE_FILES)

CC						:= clang
LINK_CMD				:= $(CC)
CFLAGS					:= -Wall -Wextra -pedantic 
LFLAGS					:= -Wall -Wextra

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

INC_DIR					:= include $(LIBFT_DIR) $(FT_PRINTF_DIR) $(MLX_DIR)

CFLAGS          		+=
LFLAGS          		+=

SOURCES					:= $(patsubst %.c,$(SRC_DIR)/%.c,$(FILE_NAMES))
OBJECTS					:= $(patsubst %.c,$(OBJ_DIR)/%.o,$(FILE_NAMES))
DEPENDS					:= $(patsubst %.c,$(DEP_DIR)/%.d,$(FILE_NAMES))

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

ifndef config
	config = distr
endif

ifndef san
	san := address
endif 

ifndef nothread
	CFLAGS		+= -DRT_MT
endif

ifeq ($(config), debug)
	CFLAGS		+= -DSH_DEBUG=1 -fno-inline -g3 -O0 -DSH_BACKTRACE
	LFLAGS		+= -DSH_DEBUG=1 -fno-inline
	ifeq ($(san), address)
		CFLAGS	+= -fsanitize=address,undefined
		LFLAGS	+= -fsanitize=address,undefined
	else ifeq ($(san), memory)
		CFLAGS	+= -fsanitize=memory,undefined
		LFLAGS	+= -fsanitize=memory,undefined
	endif
else ifeq ($(config), release)
	CFLAGS		+= -g3 -O2
	LFLAGS		+=
else ifeq ($(config), distr)
	CFLAGS		+= -g0 -Ofast -flto
	LFLAGS		+= -g0 -Ofast -flto
else
$(error "invalid config $(config"))
endif

all: $(NAME)

SILENT			:=

ifndef verbose
	SILENT		:= @
endif

ifeq ($(platform), macos)
	FRAMEWORKS	:= -framework OpenGL -framework AppKit
else
	FRAMEWORKS	:= -lX11 -lXext -lm
	CFLAGS		+= -DRT_LINUX
endif

$(NAME): $(OBJECTS) $(LIBFT_LIB) $(FT_PRINTF_LIB) $(MLX_LIB)
	@printf $(LINK_COLOR)Linking$(RESET)\ $(OBJECT_COLOR)$(notdir $@)$(RESET)\\n
	$(SILENT)$(LINK_CMD) -o $@ $(OBJECTS) $(LIBFT_LIB) $(FT_PRINTF_LIB) $(MLX_LIB) $(FRAMEWORKS) $(LFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(SILENT)mkdir -p $(@D)
	@printf $(COMPILE_COLOR)Compiling$(RESET)\ $(notdir $<)\\n
	$(SILENT)$(CC) $(CFLAGS) -c -o $@ $< -MMD $(patsubst %,-I%,$(INC_DIR))

$(LIBFT_LIB):
	$(SILENT)${MAKE} -C $(LIBFT_DIR) bonus

$(FT_PRINTF_LIB):
	$(SILENT)${MAKE} -C $(FT_PRINTF_DIR)

$(MLX_LIB):
	$(SILENT)${MAKE} -C $(MLX_DIR)

clean:
	@printf $(CLEAN_COLOR)Cleaning\ object\ files\ and\ dependencies$(RESET)\\n
	$(SILENT)rm -rf build
	$(SILENT)${MAKE} -C $(LIBFT_DIR) fclean
	$(SILENT)${MAKE} -C $(FT_PRINTF_DIR) fclean
	$(SILENT)${MAKE} -C $(MLX_DIR) clean

fclean: clean
	@printf $(CLEAN_COLOR)Cleaning\ output\ files$(RESET)\\n
	$(SILENT)rm -f $(NAME)
	$(SILENT)rm -f crash

re: fclean
	make all

-include $(DEPENDS)
.PHONY: all clean fclean re
