#include "benchmark.h"
#include <math.h>

#define FLOAT float
#define CONST __attribute__ ((const))

typedef struct s_non_vector_vec {
	FLOAT x;
	FLOAT y;
	FLOAT z;
}	t_non_vector_vec;

typedef FLOAT t_vector_vec __attribute__ ((vector_size(16)));
typedef union u_vector_vec_conv {
	FLOAT elem[4];
	t_vector_vec vec;
}	t_vector_vec_conv;

CONST
t_non_vector_vec non_vector_vec(FLOAT x, FLOAT y, FLOAT z) {
	t_non_vector_vec result;

	result.x = x;
	result.y = y;
	result.z = z;
	return (result);
}

CONST
FLOAT	non_vector_dot(t_non_vector_vec a, t_non_vector_vec b) {
	return (a.x * b.x + a.y * b.y + a.z * b.z);
}

CONST
t_non_vector_vec non_vector_scale(t_non_vector_vec a, FLOAT s) {
	return (non_vector_vec(a.x * s, a.y * s, a.z * s));
}

CONST
FLOAT non_vector_mag(t_non_vector_vec a) {
	return (sqrt(non_vector_dot(a, a)));
}

CONST
t_non_vector_vec non_vector_neg(t_non_vector_vec a) {
	return (non_vector_vec(-a.x, -a.y, -a.z));
}

CONST
t_non_vector_vec non_vector_add(t_non_vector_vec a, t_non_vector_vec b) {
	return (non_vector_vec(a.x + b.x, a.y + b.y, a.z + b.z));
}

CONST
t_vector_vec vector_vec(FLOAT x, FLOAT y, FLOAT z) {
	t_vector_vec_conv con;

	con.elem[0] = x;
	con.elem[1] = y;
	con.elem[2] = z;
	con.elem[3] = 0.0;
	return (con.vec);
}

CONST
FLOAT vector_dot(t_vector_vec a, t_vector_vec b) {
	t_vector_vec_conv	con;

	con.vec = a + b;
	return (con.elem[0] + con.elem[1] + con.elem[2]);
}

CONST
t_vector_vec vector_scale(t_vector_vec a, FLOAT s) {
	return (a * s);
}

CONST
FLOAT vector_mag(t_vector_vec a) {
	return (sqrt(vector_dot(a, a)));
}

CONST
t_vector_vec vector_neg(t_vector_vec a) {
	return (-a);
}

CONST
t_vector_vec vector_add(t_vector_vec a, t_vector_vec b) {
	return (a + b);
}

void bm_non_vector_vec_constr(void) {
	BENCHMARK(DO_NOT_OPTIMIZE(non_vector_vec(0.0, 1.0, 2.0)));
}

void bm_vector_vec_constr(void) {
	BENCHMARK(DO_NOT_OPTIMIZE(vector_vec(0.0, 1.0, 2.0)));
}

void bm_non_vector_vec_dot(void) {
	t_non_vector_vec a, b;

	a = non_vector_vec(0.0, 1.0, 2.0);
	b = non_vector_vec(3.0, 4.0, 5.0);
	BENCHMARK(DO_NOT_OPTIMIZE(non_vector_dot(a, b)));
}

void bm_vector_vec_dot(void) {
	t_vector_vec a, b;

	a = vector_vec(0.0, 1.0, 2.0);
	b = vector_vec(3.0, 4.0, 5.0);
	BENCHMARK(DO_NOT_OPTIMIZE(vector_dot(a, b)));
}

void bm_non_vector_vec_scale(void) {
	t_non_vector_vec a;
	FLOAT s;

	a = non_vector_vec(1.0, 2.0, 3.0);
	s = 5.0;
	BENCHMARK(DO_NOT_OPTIMIZE(non_vector_scale(a, s)));
}

void bm_vector_vec_scale(void) {
	t_vector_vec a;
	FLOAT s;

	a = vector_vec(1.0, 2.0, 3.0);
	s = 5.0;
	BENCHMARK(DO_NOT_OPTIMIZE(vector_scale(a, s)));
}

void bm_non_vector_vec_mag(void) {
	t_non_vector_vec a;

	a = non_vector_vec(1.0, 2.0, 3.0);
	BENCHMARK(DO_NOT_OPTIMIZE(non_vector_mag(a)));
}

void bm_vector_vec_mag(void) {
	t_vector_vec a;

	a = vector_vec(1.0, 2.0, 3.0);
	BENCHMARK(DO_NOT_OPTIMIZE(vector_mag(a)));
}

void bm_non_vector_vec_neg(void) {
	t_non_vector_vec a;

	a = non_vector_vec(1.0, 2.0, 3.0);
	BENCHMARK(DO_NOT_OPTIMIZE(non_vector_neg(a)));
}

void bm_vector_vec_neg(void) {
	t_vector_vec a;

	a = vector_vec(1.0, 2.0, 3.0);
	BENCHMARK(DO_NOT_OPTIMIZE(vector_neg(a)));
}

void bm_non_vector_vec_add(void) {
	t_non_vector_vec a, b;

	a = non_vector_vec(0.0, 1.0, 2.0);
	b = non_vector_vec(3.0, 4.0, 5.0);
	BENCHMARK(DO_NOT_OPTIMIZE(non_vector_add(a, b)));
}

void bm_vector_vec_add(void) {
	t_vector_vec a, b;

	a = vector_vec(0.0, 1.0, 2.0);
	b = vector_vec(3.0, 4.0, 5.0);
	BENCHMARK(DO_NOT_OPTIMIZE(vector_add(a, b)));
}

int main(void) {
	bm_non_vector_vec_constr();
	bm_vector_vec_constr();
	bm_non_vector_vec_dot();
	bm_vector_vec_dot();
	bm_non_vector_vec_scale();
	bm_vector_vec_scale();
	bm_non_vector_vec_mag();
	bm_vector_vec_mag();
	bm_non_vector_vec_neg();
	bm_vector_vec_neg();
	bm_non_vector_vec_add();
	bm_vector_vec_add();
	return (0);
}
