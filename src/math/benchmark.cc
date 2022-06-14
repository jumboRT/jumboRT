#include <stdio.h>
#include <time.h>
#include <benchmark/benchmark.h>
#include <smmintrin.h>

class SimpleVector {
	float x, y, z;

public:
	SimpleVector(float x, float y, float z) : x(x), y(y), z(z) {

	}

	static float Dot(const SimpleVector &a, const SimpleVector &b) {
		return (a.x * b.x + a.y * b.y + a.z * b.z);
	}
};

class SSEVector {
	__m128 v;

public:
	SSEVector(float x, float y, float z) {

	}

	static float Dot(const SSEVector &a, const SSEVector &b) {
		float	res[4];
		__m128	dot;

		dot = _mm_dp_ps(a.v, b.v, 0xFF);
		_mm_store_ps(res, dot);
		return res[0];
	}
};


static void BM_vec_dot(benchmark::State &state) {
	SimpleVector a(1.0, 2.0, 3.0);
	SimpleVector b(4.0, 5.0, 6.0);

	for (auto _ : state)
		benchmark::DoNotOptimize(SimpleVector::Dot(a, b));
}

BENCHMARK(BM_vec_dot);

static void BM_vec_dot_sse(benchmark::State &state) {
	SSEVector a(1.0, 2.0, 3.0);
	SSEVector b(4.0, 5.0, 6.0);

	for (auto _ : state)
		benchmark::DoNotOptimize(SSEVector::Dot(a, b));
}

BENCHMARK(BM_vec_dot_sse);

BENCHMARK_MAIN();
