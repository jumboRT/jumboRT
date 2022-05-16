SRC=\
	ft_atoi.c 		ft_bzero.c 		ft_calloc.c 	ft_isalnum.c	\
	ft_isalpha.c 	ft_isascii.c 	ft_isdigit.c 	ft_isprint.c	\
	ft_itoa.c 		ft_memchr.c 	ft_memcmp.c 	ft_memcpy.c		\
	ft_memmove.c 	ft_memset.c 	ft_putchar_fd.c	ft_putendl_fd.c	\
	ft_putnbr_fd.c 	ft_putstr_fd.c 	ft_split.c 		ft_strchr.c		\
	ft_strdup.c 	ft_striteri.c 	ft_strjoin.c 	ft_strlcat.c	\
	ft_strlcpy.c 	ft_strlen.c 	ft_strmapi.c 	ft_strncmp.c	\
	ft_strnstr.c 	ft_strrchr.c 	ft_strtrim.c 	ft_substr.c		\
	ft_tolower.c 	ft_toupper.c	shft_strndup.c	shft_atol.c		\
	shft_strcmp.c	shft_isblank.c	shft_iscntrl.c	shft_isgraph.c	\
	shft_islower.c	shft_ispunct.c	shft_isupper.c	shft_isxdigit.c \
	shft_isspace.c
SRC_BONUS=\
	ft_lstadd_back_bonus.c 	ft_lstadd_front_bonus.c	\
	ft_lstclear_bonus.c 	ft_lstdelone_bonus.c	\
	ft_lstiter_bonus.c 		ft_lstlast_bonus.c		\
	ft_lstmap_bonus.c 		ft_lstnew_bonus.c		\
	ft_lstsize_bonus.c		shft_lstforeach.c
INC=\
	libft.h
OBJ=$(SRC:.c=.o)
OBJ_BONUS=$(SRC_BONUS:.c=.o)
CFLAGS=-Wall -Wextra -Werror -DFT_SAFE=1 -ansi -pedantic -Ofast
NAME=libft.a

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

SILENT					:=
ifndef verbose
	SILENT				:= @
endif

all: $(NAME)

bonus: $(NAME) $(OBJ_BONUS)
	@printf $(LINK_COLOR)Archiving\ bonus$(RESET)\ $(OBJECT_COLOR)$(notdir $(NAME))$(RESET)\\n
	$(SILENT)ar rcs $(NAME) $(OBJ_BONUS)

$(NAME): $(INC) $(OBJ)
	@printf $(LINK_COLOR)Archiving$(RESET)\ $(OBJECT_COLOR)$(notdir $(NAME))$(RESET)\\n
	$(SILENT)ar rcs $(NAME) $(OBJ)

clean:
	@printf $(CLEAN_COLOR)Cleaning\ libft\ object\ files$(RESET)\\n
	$(SILENT)rm -f $(OBJ) $(OBJ_BONUS)

fclean: clean
	@printf $(CLEAN_COLOR)Cleaning\ libft\ output\ files$(RESET)\\n
	$(SILENT)rm -f $(NAME)

%.o: %.c $(INC)
	@printf $(COMPILE_COLOR)Compiling$(RESET)\ $(notdir $<)\\n
	$(SILENT)$(CC) -c $(CFLAGS) -o $@ $<

norm:
	norminette $(SRC) $(SRC_BONUS) $(INC)

test: bonus
	if [ ! -d ~/goinfre/test42 ]; then git clone https://github.com/DutChen18/test42 ~/goinfre/test42; fi
	(cd ~/goinfre/test42 && ./test.py test libft $(shell pwd)/libft.a)

re: fclean
	make all