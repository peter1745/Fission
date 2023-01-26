#pragma once

#if defined(__AVX2__) && !defined(FISSION_USE_AVX2)
	#define FISSION_USE_AVX2
#endif

#if (defined(__AVX__) || defined(FISSION_USE_AVX2)) && !defined(FISSION_USE_AVX)
	#define FISSION_USE_AVX
#endif

#if defined(FISSION_USE_AVX) && !defined(FISSION_USE_SSE)
	#define FISSION_USE_SSE

	#include <immintrin.h>
#endif
