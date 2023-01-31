#pragma once

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
	#if defined(WIN64) || defined(_WIN64)
		#define FISSION_PLATFORM_WINDOWS
	#else
		#error Fission only supports 64-bit Windows
	#endif
#elif __linux__ || __unix__
	#define FISSION_PLATFORM_LINUX
#endif

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
