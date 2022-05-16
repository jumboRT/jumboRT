#include <rtmath.h>

#include <ft_printf.h>
#include <unistd.h>

static void
	dbg_float(FLOAT a)
{
	ft_fprintf(STDERR_FILENO, "%d.%03d", (int) a, (int)(a * 1000) % 1000);
}

void
	dbg_vec(t_vec a)
{
	ft_fprintf(STDERR_FILENO, "vec(");
	dbg_float(a.v[X]);
	ft_fprintf(STDERR_FILENO, ", ");
	dbg_float(a.v[Y]);
	ft_fprintf(STDERR_FILENO, ", ");
	dbg_float(a.v[Z]);
	ft_fprintf(STDERR_FILENO, ", ");
	dbg_float(a.v[W]);
	ft_fprintf(STDERR_FILENO, ")");
}

void
	dbg_mat(t_mat a)
{
	ft_fprintf(STDERR_FILENO, "mat(");
	dbg_vec(a.rows[0]);
	ft_fprintf(STDERR_FILENO, "\n    ");
	dbg_vec(a.rows[1]);
	ft_fprintf(STDERR_FILENO, "\n    ");
	dbg_vec(a.rows[2]);
	ft_fprintf(STDERR_FILENO, "\n    ");
	dbg_vec(a.rows[3]);
	ft_fprintf(STDERR_FILENO, ")\n");
}
