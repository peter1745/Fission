#pragma once

#include "Core.hpp"

#include <format>

#define FISSION_ENABLE_VERIFY 1

#if defined(FISSION_PLATFORM_WINDOWS)
	#define FISSION_DEBUG_BREAK __debugbreak()
#else
	#include <signal.h>
	#define FISSION_DEBUG_BREAK raise(SIGABRT)
#endif

#define FISSION_VERIFY(cond, ...) { if (!(cond)) { FISSION_DEBUG_BREAK; } }
