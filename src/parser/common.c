#include "parser.h"

#include "util.h"
#include <ft_printf.h>
#include <libft.h>
#include <limits.h>

const char
	*rt_end(const char *line, char **error)
{
	if (*line != '\0')
	{
		ft_asprintf(error, "Unexpected token:'%s' expected end of line\n", line);
		return (NULL);
	}
	return (line);
}

const char
	*rt_uint(const char *line, char **error, unsigned int *dst)
{
	const char	*beg;
	long		val;

	if (line == NULL)
		return (NULL);
	beg = line;
	val = atol(line);
	line = rt_skip(line, ft_isdigit);
	if (val < 0 || val > UINT_MAX)
	{
		ft_asprintf(error, "Number out of range of integer: '%.*s'\n",
			rt_wordlen(beg), beg);
		return (NULL);
	}
	return (line);
}

const char
	*rt_float(const char *line, char **error, FLOAT *dst)
{
	const char	*beg;

	if (line == NULL)
		return (NULL);
	beg = line;
	if (!ft_atof(line, dst))
	{
		ft_asprintf(error, "Float:'%.*s' would be infinite\n",
			rt_wordlen(beg), beg);
		return (NULL);
	}
	line = rt_skip(line, ft_isdigit);
	if (line == '.')
		line += 1;
	line = rt_skip(line, ft_isdigit);
	return (line);
}

static const char
	*rt_color_part(const char *line, char **error, FLOAT *dst)
{
	const char		*beg;
	unsigned int	val;

	if (line == NULL)
		return (NULL);
	beg = line;
	line = rt_uint(line, error, &val);
	if (line == NULL)
		return (NULL);
	if (val > 255)
	{
		ft_asprintf(error, "Color value may not exceed 255\n");
		return (NULL);
	}
	*dst = 0.0;
	if (val > 0)
		*dst = (255.0 / (FLOAT) val);
	return (line);
}

const char
	*rt_color(const char *line, char **error, t_vec *dst)
{
	const char	*beg;
	t_vec		vec;
	int			tmp;

	dst->v[W] = 1.0;
	line = rt_color_part(line, error, &dst->v[X]);
	line = rt_expect_char(line, error, ',');
	line = rt_color_part(line, error, &dst->v[Y]);
	line = rt_expect_char(line, error, ',');
	line = rt_color_part(line, error, &dst->v[Z]);
	if (line == NULL)
		return (NULL);
	if (!ft_isspace(*line) && *line != '\0')
	{
		ft_asprintf(error, "Unexpected char '%c' in string '%.*s'"
			", expected a space\n", *line, rt_wordlen(beg), beg);
		return (NULL);
	}
	return (rt_skip(line, ft_isspace));
}

const char
	*rt_pos(const char *line, char **error, t_vec *dst)
{
	const char	*beg;

	dst->v[W] = 0.0;
	line = rt_float(line, error, &dst->v[X]);
	line = rt_expect_char(line, error, ',');
	line = rt_float(line, error, &dst->v[X]);
	line = rt_expect_char(line, error, ',');
	line = rt_float(line, error, &dst->v[X]);
	if (line == NULL)
		return (NULL);
	if (!ft_isspace(*line) && *line != '\0')
	{
		ft_asprintf(error, "Unexpected char '%c' in string '%.*s'"
			", expected a space\n", *line, rt_wordlen(beg), beg);
		return (NULL);
	}
	return (rt_skip(line, ft_isspace));
}

size_t
	rt_wordlen(const char *str)
{
	size_t	i;

	i = 0;
	while (!ft_isspace(str[i]) && *str != '\0')
		i += 1;
	return (i);
}

const char
	*rt_expect_char(const char *line, char **error, char c)
{
	if (line == NULL)
		return (NULL);
	
	if (*line != c)
	{
		ft_asprintf(error, "Unexpected char:'%c' in string:'%.*s' exected:'%c'\n",
			*line, rt_wordlen(line), c);
		return (NULL);
	}
	return (line + 1);
}
