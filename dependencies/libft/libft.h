/* ************************************************************************** */
/*                                                                            */
/*                                                           :      .         */
/*   libft.h                                          -=-:::+*+:-+*#.         */
/*                                                :-:::+#***********----:     */
/*   By: csteenvo <csteenvo@student.codam.n>        .:-*#************#-       */
/*                                                 :=+*+=+*********####+:     */
/*   Created: 2022/05/10 12:00:51 by csteenvo     ..     +**=-=***-           */
/*   Updated: 2022/05/10 12:00:51 by csteenvo            :      ..            */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H
# include <stddef.h>

# ifndef FT_SAFE
#  define FT_SAFE	0
# endif

typedef struct s_list	t_list;

struct s_list
{
	void	*content;
	t_list	*next;
};

/* characters */
int		ft_isalpha(int ch);
int		ft_isdigit(int ch);
int		ft_isalnum(int ch);
int		ft_isascii(int ch);
int		ft_isprint(int ch);
int		ft_isgraph(int ch);
int		ft_isblank(int ch);
int		ft_iscntrl(int ch);
int		ft_islower(int ch);
int		ft_isupper(int ch);
int		ft_ispunct(int ch);
int		ft_isspace(int ch);
int		ft_isxdigit(int ch);
int		ft_toupper(int ch);
int		ft_tolower(int ch);

/* strings */
size_t	ft_strlen(const char *str);
size_t	ft_strlcpy(char *dst, const char *src, size_t size);
size_t	ft_strlcat(char *dst, const char *src, size_t size);
char	*ft_strchr(const char *str, int ch);
char	*ft_strrchr(const char *str, int ch);
int		ft_strncmp(const char *str1, const char *str2, size_t len);
char	*ft_strnstr(const char *str1, const char *str2, size_t len);
char	*ft_strstr(const char *str1, const char *str2);
int		ft_atoi(const char *str);
long	ft_atol(const char *str);
int		ft_strcmp(const char *str1, const char *str2);

/* memory */
void	*ft_memset(void *ptr, int ch, size_t size);
void	ft_bzero(void *ptr, size_t size);
void	*ft_memcpy(void *dst, const void *src, size_t size);
void	*ft_memmove(void *dst, const void *src, size_t size);
void	*ft_memchr(const void *ptr, int ch, size_t size);
int		ft_memcmp(const void *ptr1, const void *ptr2, size_t size);

/* malloc */
void	*ft_calloc(size_t count, size_t size);
char	*ft_strdup(const char *str);
char	*ft_strndup(const char *str, size_t n);

/* strings 2 */
char	*ft_substr(const char *str, unsigned int idx, size_t max);
char	*ft_strjoin(const char *str1, const char *str2);
char	*ft_strtrim(const char *str1, const char *str2);
char	**ft_split(const char *str, char ch);
char	*ft_itoa(int num);
char	*ft_strmapi(const char *str, char (*func)(unsigned int, char));
void	ft_striteri(char *str, void (*func)(unsigned int, char*));

/* io */
int		ft_putchar_fd(char ch, int fd);
int		ft_putstr_fd(const char *str, int fd);
int		ft_putendl_fd(const char *str, int fd);
void	ft_putnbr_fd(int num, int fd);

/* lists */
t_list	*ft_lstnew(void *content);
void	ft_lstadd_front(t_list **lst, t_list *new);
int		ft_lstsize(t_list *lst);
t_list	*ft_lstlast(t_list *lst);
void	ft_lstadd_back(t_list **lst, t_list *new);
void	ft_lstdelone(t_list *lst, void (*del)(void *));
void	ft_lstclear(t_list **lst, void (*del)(void *));
void	ft_lstiter(t_list *lst, void (*func)(void *));
void	ft_lstforeach(const t_list *lst,
			void (*proc)(void *, void *), void *param);
t_list	*ft_lstmap(t_list *lst, void *(*func)(void *), void (*del)(void *));
#endif
