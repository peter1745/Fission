#pragma once

#include "../simpletest.h"

#include <Fission/Math/Vec4.hpp>

using namespace Fission::Math;

DEFINE_TEST_G(Addition, Vector4)
{
	Vec4 A(5.0f, 0.5f, -10.0f, 1.0f);
	Vec4 B(1.0f, 1.0f, -5.0f, 1.0f);
	Vec4 Expected(6.0f, 1.5f, -15.0f, 2.0f);
	Vec4 Res = A + B;

	TEST(Res == Expected);
}

DEFINE_TEST_G(Subtraction, Vector4)
{
	Vec4 A(5.0f, 0.5f, -10.0f, 1.0f);
	Vec4 B(1.0f, 1.0f, -5.0f, 1.0f);
	Vec4 Expected(4.0f, -0.5f, -5.0f, 0.0f);
	Vec4 Res = A - B;
	TEST(Res == Expected);
}

DEFINE_TEST_G(Division, Vector4)
{
	Vec4 A(5.0f, 0.5f, -10.0f, 1.0f);
	Vec4 B(1.0f, 1.0f, -5.0f, 1.0f);
	Vec4 Expected(5.0f, 0.5f, 2.0f, 1.0f);
	Vec4 Res = A / B;

	TEST(Res == Expected);
}

DEFINE_TEST_G(Multiplication, Vector4)
{
	Vec4 A(5.0f, 0.5f, -10.0f, 1.0f);
	Vec4 B(1.0f, 1.0f, -5.0f, 1.0f);
	Vec4 Expected(5.0f, 0.5f, 50.0f, 1.0f);
	Vec4 Res = A * B;

	TEST(Res == Expected);
}

DEFINE_TEST_G(Length, Vector4)
{
	Vec4 A(5.0f, 0.5f, -10.0f, 1.0f);
	TEST_EQ(A.LengthSq(), (5.0f * 5.0f) + (0.5f * 0.5f) + (-10.0f * -10.0f) + (1.0f * 1.0f));
	TEST_EQ(A.Length(), sqrt((5.0f * 5.0f) + (0.5f * 0.5f) + (-10.0f * -10.0f) + (1.0f * 1.0f)));
}

DEFINE_TEST_G(Normalize, Vector4)
{
	const float AllowedError = 0.001f;
	Vec4 A(5.0f, 0.5f, -10.0f, 1.0f);
	Vec4 B = A.Normalized();
	A.Normalize();
	TEST_LESS_EQUAL(1.0f - A.Length(), AllowedError);
	TEST_LESS_EQUAL(1.0f - B.Length(), AllowedError);
}
