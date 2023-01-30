#include "FissionPCH.hpp"
#include "FQuat.hpp"

namespace Fission::Math {

	FQuat::FQuat(float InX, float InY, float InZ, float InW)
	    : Value(InX, InY, InZ, InW)
	{
	}

	FQuat::FQuat(const FVec4& InValue)
	    : Value(InValue)
	{
	}

	FQuat::FQuat(const FVec3& InAxis, float InAngle)
	{
		float halfAngle = InAngle * 0.5f;
		float halfSin = sinf(halfAngle);
		auto normalizedAxis = InAxis.Normalized();

		Value = FVec4(normalizedAxis.X * halfSin, normalizedAxis.Y * halfSin, normalizedAxis.Z * halfSin, cosf(halfAngle));
	}

	FQuat FQuat::operator*(const FQuat& InOther) const
	{
		float x0 = Value.X;
		float y0 = Value.Y;
		float z0 = Value.Z;
		float w0 = Value.W;

		float x1 = InOther.Value.X;
		float y1 = InOther.Value.Y;
		float z1 = InOther.Value.Z;
		float w1 = InOther.Value.W;

		float x = w0 * x1 + x0 * w1 + y0 * z1 - z0 * y1;
		float y = w0 * y1 - x0 * z1 + y0 * w1 + z0 * x1;
		float z = w0 * z1 + x0 * y1 - y0 * x1 + z0 * w1;
		float w = w0 * w1 - x0 * x1 - y0 * y1 - z0 * z1;

		return FQuat(x, y, z, w);
	}

	FVec3 FQuat::operator*(const FVec3& InValue) const
	{
		return FVec3((*this * FQuat(FVec4(InValue, 0.0f)) * Conjugated()).Value);
	}

	FQuat FQuat::Conjugated() const
	{
		return FQuat(-Value.X, -Value.Y, -Value.Z, Value.W);
	}

}
