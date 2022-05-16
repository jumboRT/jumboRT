SRC=\
	ft_printf.c \
	ft_printf_parse.c \
	ft_printf_string.c \
	ft_printf_integer.c \
	ft_printf_utils.c \
	ft_printf_superfluous.c
INC=\
	ft_printf.h
OBJ=$(SRC:.c=.o)
CFLAGS=-Wall -Wextra -Werror -ansi -pedantic
NAME=libftprintf.a

BLACK			:="\033[0;30m"
RED				:="\033[0;31m"
GREEN			:="\033[0;32m"
ORANGE			:="\033[0;33m"
BLUE			:="\033[0;34m"
PURPLE			:="\033[0;35m"
CYAN			:="\033[0;36m"
LIGHT_GRAY		:="\033[0;37m"

DARK_GRAY		:="\033[1;30m"
LIGHT_RED		:="\033[1;31m"
LIGHT_GREEN		:="\033[1;32m"
YELLOW			:="\033[1;33m"
LIGHT_BLUE		:="\033[1;34m"
LIGHT_PURPLE	:="\033[1;35m"
LIGHT_CYAN		:="\033[1;36m"
WHITE			:="\033[1;37m"

RESET			:="\033[0m"

COMPILE_COLOR	:= $(GREEN)
LINK_COLOR		:= $(CYAN)
OBJECT_COLOR	:= $(RED)
CLEAN_COLOR				:= $(PURPLE)

SILENT			:=
ifndef verbose
	SILENT		:= @
endif

all: $(NAME)
bonus: $(NAME)

$(NAME): $(INC) $(OBJ)
	@printf $(LINK_COLOR)Archiving$(RESET)\ $(OBJECT_COLOR)$(notdir $@)$(RESET)\\n
	$(SILENT)ar rcs $(NAME) $(OBJ)

clean:
	@printf $(CLEAN_COLOR)Cleaning\ ft_printf\ object\ files$(RESET)\\n
	$(SILENT)rm -f $(OBJ)

fclean: clean
	@printf $(CLEAN_COLOR)Cleaning\ ft_printf\ output\ files$(RESET)\\n
	$(SILENT)rm -f $(NAME)

%.o: %.c $(INC)
	@printf $(COMPILE_COLOR)Compiling$(RESET)\ $(notdir $<)\\n
	$(SILENT)$(CC) -c $(CFLAGS) -o $@ $<

norm:
	norminette $(SRC) $(INC)

test: bonus
	if [ ! -d ~/goinfre/test42 ]; then git clone https://github.com/DutChen18/test42 ~/goinfre/test42; fi
	(cd ~/goinfre/test42 && ./test.py test ft_printf $(shell pwd)/libftprintf.a)

re: fclean
	make all