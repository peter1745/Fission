#pragma once

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
	#if defined(_WIN64)
		#define TESTFRAMEWORK_PLATFORM_WINDOWS
	#else
		#error TestFramework only supports 64-bit Windows platforms!
	#endif
#elif __linux__
	#define TESTFRAMEWORK_PLATFORM_LINUX
#endif

#if defined(TESTFRAMEWORK_PLATFORM_WINDOWS)
#define TESTFRAMEWORK_DEBUG_BREAK __debugbreak()
#else
#include <signal.h>
#define TESTFRAMEWORK_DEBUG_BREAK raise(SIGABRT)
#endif

#define TESTFRAMEWORK_VERIFY(cond, ...) { if (!(cond)) { TESTFRAMEWORK_DEBUG_BREAK; } }

#include <cstdint>
#include <memory>

#define FUSION_BIND_FUNC(Func) [this](auto&&... InArgs) -> decltype(auto) { return this->Func(std::forward<decltype(InArgs)>(InArgs)...); }

namespace TestFramework {

	using Byte = uint8_t;

	template<typename T>
	using Unique = std::unique_ptr<T>;
	template<typename T, typename... TArgs>
	Unique<T> MakeUnique(TArgs&&... InArgs) { return std::make_unique<T>(std::forward<TArgs>(InArgs)...); }

	template<typename T>
	constexpr T Shift1(T InShift) { return T(1) << InShift; }

}
