#include "parser.h"

#include "util.h"
#include <ft_printf.h>
#include <libft.h>
#include <limits.h>


#include <stdio.h>

const char
	*rt_end(const char *line, char **error)
{
	if (line == NULL)
		return (NULL);
	if (*line != '\0' && *line != '\n')
	{
		ft_asprintf(error, "Unexpected token:'%s' expected end of line", line);
		return (NULL);
	}
	return (line);
}

/* TODO: empty string is incorrectly parsed as 0 */
const char
	*rt_uint(const char *line, char **error, unsigned int *dst)
{
	const char	*beg;
	long		val;

	if (line == NULL)
		return (NULL);
	if (!ft_isdigit(*line) && *line != '-')
	{
		ft_asprintf(error, "Expected digit, found '%.*s'",
			(int) rt_wordlen(beg), beg);
		return (NULL);
	}
	beg = line;
	val = ft_atol(line);
	line = rt_skip(line, ft_isdigit);
	if (val < 0 || val > UINT_MAX)
	{
		ft_asprintf(error, "Number out of range of integer: '%.*s'",
			(int) rt_wordlen(beg), beg);
		return (NULL);
	}
	*dst = (unsigned int) val;
	return (line);
}

/* TODO: empty string is incorrectly parsed as 0 */
const char
	*rt_float(const char *line, char **error, FLOAT *dst)
{
	const char	*beg;

	if (line == NULL)
		return (NULL);
	line = rt_skip(line, ft_isspace);
	if (!ft_isdigit(*line))
	{
		ft_asprintf(error, "Expected digit, found '%.*s'",
			(int) rt_wordlen(beg), beg);
		return (NULL);
	}
	beg = line;
	if (!rt_atof(line, dst))
	{
		ft_asprintf(error, "Float:'%.*s' would be infinite",
			(int) rt_wordlen(beg), beg);
		return (NULL);
	}
	if (*line == '-' || *line == '+')
		line += 1;
	line = rt_skip(line, ft_isdigit);
	if (*line == '.')
		line += 1;
	line = rt_skip(line, ft_isdigit);
	return (line);
}

static const char
	*rt_color_part(const char *line, char **error, FLOAT *dst)
{
	unsigned int	val;

	if (line == NULL)
		return (NULL);
	line = rt_uint(line, error, &val);
	if (line == NULL)
		return (NULL);
	if (val > 255)
	{
		ft_asprintf(error, "Color value may not exceed 255");
		return (NULL);
	}
	*dst = 0.0;
	if (val > 0)
		*dst = (val / 255.0);
	return (line);
}

const char
	*rt_color(const char *line, char **error, t_vec *dst)
{
	if (line == NULL)
		return (NULL);
	line = rt_skip(line, ft_isspace);
	dst->v[W] = 1.0;
	line = rt_color_part(line, error, &dst->v[X]);
	line = rt_expect_char(line, error, ',');
	line = rt_color_part(line, error, &dst->v[Y]);
	line = rt_expect_char(line, error, ',');
	line = rt_color_part(line, error, &dst->v[Z]);
	if (line == NULL)
		return (NULL);
	return (line);
}

const char
	*rt_pos(const char *line, char **error, t_vec *dst)
{
	if (line == NULL)
		return (NULL);
	line = rt_skip(line, ft_isspace);
	dst->v[W] = 0.0;
	line = rt_float(line, error, &dst->v[X]);
	line = rt_expect_char(line, error, ',');
	line = rt_float(line, error, &dst->v[Y]);
	line = rt_expect_char(line, error, ',');
	line = rt_float(line, error, &dst->v[Z]);
	if (line == NULL)
		return (NULL);
	return (rt_skip(line, ft_isspace));
}

const char
	*rt_norm_vec(const char *line, char **error, t_vec *dst)
{
	const char	*beg;

	if (line == NULL)
		return (NULL);
	line = rt_skip(line, ft_isspace);
	beg = line;
	dst->v[W] = 0.0;
	line = rt_float(line, error, &dst->v[X]);
	line = rt_expect_char(line, error, ',');
	line = rt_float(line, error, &dst->v[Y]);
	line = rt_expect_char(line, error, ',');
	line = rt_float(line, error, &dst->v[Z]);
	if (line == NULL)
		return (NULL);
	if (vec_mag(*dst) < 0.99 || vec_mag(*dst) > 1.01)
	{
		ft_asprintf(error, "Vector '%.*s' is not a normalized vector",
				(int) rt_wordlen(beg), beg);
		return (NULL);
	}
	return (rt_skip(line, ft_isspace));
}

size_t
	rt_wordlen(const char *str)
{
	size_t	i;

	i = 0;
	while (str[i] != '\0' && !ft_isspace(str[i]))
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
		ft_asprintf(error, "Unexpected char:'%c' in string:'%.*s' exected:'%c'",
			*line, (int) rt_wordlen(line), line, c);
		return (NULL);
	}
	return (line + 1);
}

const char
	*rt_next_word(const char *line)
{
	const char	*next;

	next = rt_skip(line, ft_isalpha);
	next = rt_skip(next, ft_isspace);
	return (next);
}

const char
	*rt_skip(const char *line, int (*skip_char)(int))
{
	if (line == NULL)
		return (NULL);
	while (skip_char(*line))
		line += 1;
	return (line);
}
