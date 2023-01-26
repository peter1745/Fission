#pragma once

#include "../simpletest.h"

#include <Fission/Math/Vec3.hpp>

#ifdef FISSION_USE_SSE
static constexpr bool s_SSEEnabled = true;
#else
static constexpr bool s_SSEEnabled = false;
#endif

#define PRINT_SSE_USAGE() LogMessage("SSE Enabled: %s", (s_SSEEnabled ? "true" : "false"))

using namespace Fission::Math;

DEFINE_TEST_G(Addition, Vector3)
{
	PRINT_SSE_USAGE();
	Vec3 A(5.0f, 0.5f, -10.0f);
	Vec3 B(1.0f, 1.0f, -5.0f);
	Vec3 Expected(6.0f, 1.5f, -15.0f);
	Vec3 Res = A + B;

	TEST(Res == Expected);
}

DEFINE_TEST_G(Subtraction, Vector3)
{
	PRINT_SSE_USAGE();
	Vec3 A(5.0f, 0.5f, -10.0f);
	Vec3 B(1.0f, 1.0f, -5.0f);
	Vec3 Expected(4.0f, -0.5f, -5.0f);
	Vec3 Res = A - B;
	TEST(Res == Expected);
}

DEFINE_TEST_G(Division, Vector3)
{
	PRINT_SSE_USAGE();
	Vec3 A(5.0f, 0.5f, -10.0f);
	Vec3 B(1.0f, 1.0f, -5.0f);
	Vec3 Expected(5.0f, 0.5f, 2.0f);
	Vec3 Res = A / B;

	TEST(Res == Expected);
}

DEFINE_TEST_G(Multiplication, Vector3)
{
	PRINT_SSE_USAGE();
	Vec3 A(5.0f, 0.5f, -10.0f);
	Vec3 B(1.0f, 1.0f, -5.0f);
	Vec3 Expected(5.0f, 0.5f, 50.0f);
	Vec3 Res = A * B;

	TEST(Res == Expected);
}

DEFINE_TEST_G(Length, Vector3)
{
	PRINT_SSE_USAGE();
	Vec3 A(5.0f, 0.5f, -10.0f);
	TEST_EQ(A.LengthSq(), (5.0f * 5.0f) + (0.5f * 0.5f) + (-10.0f * -10.0f));
	TEST_EQ(A.Length(), sqrt((5.0f * 5.0f) + (0.5f * 0.5f) + (-10.0f * -10.0f)));
}
