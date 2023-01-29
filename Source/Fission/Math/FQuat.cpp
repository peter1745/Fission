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
		float HalfAngle = InAngle * 0.5f;
		float HalfSin = sinf(HalfAngle);
		auto NormalizedAxis = InAxis.Normalized();

		Value = FVec4(NormalizedAxis.X * HalfSin, NormalizedAxis.Y * HalfSin, NormalizedAxis.Z * HalfSin, cosf(HalfAngle));
	}

	FQuat FQuat::operator*(const FQuat& InOther) const
	{
		float X0 = Value.X;
		float Y0 = Value.Y;
		float Z0 = Value.Z;
		float W0 = Value.W;

		float X1 = InOther.Value.X;
		float Y1 = InOther.Value.Y;
		float Z1 = InOther.Value.Z;
		float W1 = InOther.Value.W;

		float X = W0 * X1 + X0 * W1 + Y0 * Z1 - Z0 * Y1;
		float Y = W0 * Y1 - X0 * Z1 + Y0 * W1 + Z0 * X1;
		float Z = W0 * Z1 + X0 * Y1 - Y0 * X1 + Z0 * W1;
		float W = W0 * W1 - X0 * X1 - Y0 * Y1 - Z0 * Z1;

		return FQuat(X, Y, Z, W);
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
