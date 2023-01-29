#include "FissionPCH.hpp"
#include "FVec3.hpp"
#include "FVec4.hpp"

namespace Fission::Math {

	FVec3::FVec3(float InScalar)
	{
		for (size_t Idx = 0; Idx < 3; Idx++)
			Values[Idx] = InScalar;
	}

	FVec3::FVec3(float InX, float InY, float InZ)
	    : X(InX), Y(InY), Z(InZ)
	{
	}

	FVec3::FVec3(const FVec4& InOther)
	    : X(InOther.X), Y(InOther.Y), Z(InOther.Z)
	{
	}

	FVec3& FVec3::operator+=(const FVec3& InOther)
	{
		for (size_t Idx = 0; Idx < 3; Idx++)
			Values[Idx] += InOther.Values[Idx];
		return *this;
	}

	FVec3& FVec3::operator+=(float InValue)
	{
		for (size_t Idx = 0; Idx < 3; Idx++)
			Values[Idx] += InValue;
		return *this;
	}

	FVec3& FVec3::operator-=(const FVec3& InOther)
	{
		for (size_t Idx = 0; Idx < 3; Idx++)
			Values[Idx] -= InOther.Values[Idx];
		return *this;
	}

	FVec3& FVec3::operator-=(float InValue)
	{
		for (size_t Idx = 0; Idx < 3; Idx++)
			Values[Idx] -= InValue;
		return *this;
	}

	FVec3& FVec3::operator/=(const FVec3& InOther)
	{
		for (size_t Idx = 0; Idx < 3; Idx++)
			Values[Idx] /= InOther.Values[Idx];
		return *this;
	}

	FVec3& FVec3::operator/=(float InValue)
	{
		for (size_t Idx = 0; Idx < 3; Idx++)
			Values[Idx] /= InValue;
		return *this;
	}

	FVec3& FVec3::operator*=(const FVec3& InOther)
	{
		for (size_t Idx = 0; Idx < 3; Idx++)
			Values[Idx] *= InOther.Values[Idx];
		return *this;
	}

	FVec3& FVec3::operator*=(float InValue)
	{
		for (size_t Idx = 0; Idx < 3; Idx++)
			Values[Idx] *= InValue;
		return *this;
	}

	FVec3 FVec3::operator+(const FVec3& InOther) const
	{
		FVec3 Result;
		for (size_t Idx = 0; Idx < 3; Idx++)
			Result.Values[Idx] = Values[Idx] + InOther.Values[Idx];
		return Result;
	}

	FVec3 FVec3::operator+(float InValue) const
	{
		FVec3 Result;
		for (size_t Idx = 0; Idx < 3; Idx++)
			Result.Values[Idx] = Values[Idx] + InValue;
		return Result;

	}

	FVec3 FVec3::operator-(const FVec3& InOther) const
	{
		FVec3 Result;
		for (size_t Idx = 0; Idx < 3; Idx++)
			Result.Values[Idx] = Values[Idx] - InOther.Values[Idx];
		return Result;
	}

	FVec3 FVec3::operator-(float InValue) const
	{
		FVec3 Result;
		for (size_t Idx = 0; Idx < 3; Idx++)
			Result.Values[Idx] = Values[Idx] - InValue;
		return Result;
	}

	FVec3 FVec3::operator/(const FVec3& InOther) const
	{
		FVec3 Result;
		for (size_t Idx = 0; Idx < 3; Idx++)
			Result.Values[Idx] = Values[Idx] / InOther.Values[Idx];
		return Result;
	}

	FVec3 FVec3::operator/(float InValue) const
	{
		FVec3 Result;
		for (size_t Idx = 0; Idx < 3; Idx++)
			Result.Values[Idx] = Values[Idx] / InValue;
		return Result;
	}

	FVec3 FVec3::operator*(const FVec3& InOther) const
	{
		FVec3 Result;
		for (size_t Idx = 0; Idx < 3; Idx++)
			Result.Values[Idx] = Values[Idx] * InOther.Values[Idx];
		return Result;
	}

	FVec3 FVec3::operator*(float InValue) const
	{
		FVec3 Result;
		for (size_t Idx = 0; Idx < 3; Idx++)
			Result.Values[Idx] = Values[Idx] * InValue;
		return Result;
	}

	FVec3 FVec3::operator-() const
	{
		return FVec3(-X, -Y, -Z);
	}

	float FVec3::Dot(const FVec3& InOther) const
	{
		float DotProduct = 0.0f;
		for (size_t Idx = 0; Idx < 3; Idx++)
			DotProduct += Values[Idx] * InOther.Values[Idx];
		return DotProduct;
	}

	FVec3 FVec3::Cross(const FVec3& InOther) const
	{
		return FVec3(
			Values[1] * InOther.Values[2] - Values[2] * InOther.Values[1],
			Values[2] * InOther.Values[0] - Values[0] * InOther.Values[2],
			Values[0] * InOther.Values[1] - Values[1] * InOther.Values[0]
		);
	}

	float FVec3::LengthSq() const
	{
		return Dot(*this);
	}

	float FVec3::Length() const
	{
		return sqrt(LengthSq());
	}

	void FVec3::Normalize()
	{
		float Len = Length();

		if (Len == 0.0f)
			return;

		float InvLength = 1.0f / Len;
		for (size_t Idx = 0; Idx < 3; Idx++)
			Values[Idx] *= InvLength;
	}

	FVec3 FVec3::Normalized() const
	{
		FVec3 Result = *this;
		Result.Normalize();
		return Result;
	}

	float& FVec3::operator[](size_t InIndex)
	{
		return Values[InIndex];
	}

	float FVec3::operator[](size_t InIndex) const
	{
		return Values[InIndex];
	}

	FVec3 FVec3::Max(const FVec3& InVec0, const FVec3& InVec1)
	{
		FVec3 Result;
		for (size_t Idx = 0; Idx < 3; Idx++)
			Result.Values[Idx] = std::max(InVec0.Values[Idx], InVec1.Values[Idx]);
		return Result;
	}

	FVec3 FVec3::Min(const FVec3& InVec0, const FVec3& InVec1)
	{
		FVec3 Result;
		for (size_t Idx = 0; Idx < 3; Idx++)
			Result.Values[Idx] = std::min(InVec0.Values[Idx], InVec1.Values[Idx]);
		return Result;
	}

	FVec3 FVec3::Zero() { return FVec3(0.0f); }

}
