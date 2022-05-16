/* ************************************************************************** */
/*                                                                            */
/*                                                           :      .         */
/*   ft_printf.h                                      -=-:::+*+:-+*#.         */
/*                                                :-:::+#***********----:     */
/*   By: csteenvo <csteenvo@student.codam.n>        .:-*#************#-       */
/*                                                 :=+*+=+*********####+:     */
/*   Created: 2022/05/10 12:00:50 by csteenvo     ..     +**=-=***-           */
/*   Updated: 2022/05/10 12:00:50 by csteenvo            :      ..            */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H
# include <stddef.h>
# include <stdarg.h>

enum e_sink_type
{
	sink_type_str,
	sink_type_fd
};

typedef struct s_flags		t_flags;
typedef struct s_sink		t_sink;
typedef enum e_sink_type	t_sink_type;

struct s_flags
{
	unsigned int	left: 1;
	unsigned int	zero: 1;
	unsigned int	alt: 1;
	unsigned int	space: 1;
	unsigned int	sign: 1;
	unsigned int	width;
	int				precision;
	int				format;
};

struct s_sink
{
	t_sink_type	type;
	char		*str;
	int			fd;
};

int	ft_printf_strlen(const char *str);
int	ft_printf_write(t_sink *sink, const char *str, int size);
int	ft_printf_repeat(t_sink *sink, char ch, int size);
int	ft_printf_chr(t_sink *sink, t_flags *flags, int value);
int	ft_printf_str(t_sink *sink, t_flags *flags, const char *value);
int	ft_printf_int(t_sink *sink, t_flags *flags, int value);
int	ft_printf_pos(t_sink *sink, t_flags *flags, size_t value);
int	ft_printf_parse(t_flags *flags, const char **fmt, va_list *args);
int	ft_printf_vxprintf(t_sink *sink, const char *fmt, va_list *args);
int	ft_printf(const char *fmt, ...);

int	ft_vprintf(const char *fmt, va_list *vlist);
int	ft_fprintf(int fd, const char *fmt, ...);
int	ft_vfprintf(int fd, const char *fmt, va_list *vlist);
int	ft_sprintf(char *buffer, const char *fmt, ...);
int	ft_vsprintf(char *buffer, const char *fmt, va_list *vlist);

#endif
