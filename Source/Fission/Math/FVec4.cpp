#include "FissionPCH.hpp"
#include "FVec4.hpp"

namespace Fission::Math {

	FVec4::FVec4(float InScalar)
	{
		for (size_t Idx = 0; Idx < 4; Idx++)
			Values[Idx] = InScalar;
	}

	FVec4::FVec4(float InX, float InY, float InZ, float InW)
	    : X(InX), Y(InY), Z(InZ), W(InW)
	{
	}

	FVec4::FVec4(const FVec3& InXYZ, float InW)
	    : X(InXYZ.X), Y(InXYZ.Y), Z(InXYZ.Z), W(InW)
	{
	}

	FVec4& FVec4::operator+=(const FVec4& InOther)
	{
		for (size_t Idx = 0; Idx < 4; Idx++)
			Values[Idx] += InOther.Values[Idx];
		return *this;
	}

	FVec4& FVec4::operator+=(float InValue)
	{
		for (size_t Idx = 0; Idx < 4; Idx++)
			Values[Idx] += InValue;
		return *this;
	}

	FVec4& FVec4::operator-=(const FVec4& InOther)
	{
		for (size_t Idx = 0; Idx < 4; Idx++)
			Values[Idx] -= InOther.Values[Idx];
		return *this;
	}

	FVec4& FVec4::operator-=(float InValue)
	{
		for (size_t Idx = 0; Idx < 4; Idx++)
			Values[Idx] -= InValue;
		return *this;
	}

	FVec4& FVec4::operator/=(const FVec4& InOther)
	{
		for (size_t Idx = 0; Idx < 4; Idx++)
			Values[Idx] /= InOther.Values[Idx];
		return *this;
	}

	FVec4& FVec4::operator/=(float InValue)
	{
		for (size_t Idx = 0; Idx < 4; Idx++)
			Values[Idx] /= InValue;
		return *this;
	}

	FVec4& FVec4::operator*=(const FVec4& InOther)
	{
		for (size_t Idx = 0; Idx < 4; Idx++)
			Values[Idx] *= InOther.Values[Idx];
		return *this;
	}

	FVec4& FVec4::operator*=(float InValue)
	{
		for (size_t Idx = 0; Idx < 4; Idx++)
			Values[Idx] *= InValue;
		return *this;
	}

	FVec4 FVec4::operator+(const FVec4& InOther) const
	{
		FVec4 Result;
		for (size_t Idx = 0; Idx < 4; Idx++)
			Result.Values[Idx] = Values[Idx] + InOther.Values[Idx];
		return Result;
	}

	FVec4 FVec4::operator+(float InValue) const
	{
		FVec4 Result;
		for (size_t Idx = 0; Idx < 4; Idx++)
			Result.Values[Idx] = Values[Idx] + InValue;
		return Result;

	}

	FVec4 FVec4::operator-(const FVec4& InOther) const
	{
		FVec4 Result;
		for (size_t Idx = 0; Idx < 4; Idx++)
			Result.Values[Idx] = Values[Idx] - InOther.Values[Idx];
		return Result;
	}

	FVec4 FVec4::operator-(float InValue) const
	{
		FVec4 Result;
		for (size_t Idx = 0; Idx < 4; Idx++)
			Result.Values[Idx] = Values[Idx] - InValue;
		return Result;
	}

	FVec4 FVec4::operator/(const FVec4& InOther) const
	{
		FVec4 Result;
		for (size_t Idx = 0; Idx < 4; Idx++)
			Result.Values[Idx] = Values[Idx] / InOther.Values[Idx];
		return Result;
	}

	FVec4 FVec4::operator/(float InValue) const
	{
		FVec4 Result;
		for (size_t Idx = 0; Idx < 4; Idx++)
			Result.Values[Idx] = Values[Idx] / InValue;
		return Result;
	}

	FVec4 FVec4::operator*(const FVec4& InOther) const
	{
		FVec4 Result;
		for (size_t Idx = 0; Idx < 4; Idx++)
			Result.Values[Idx] = Values[Idx] * InOther.Values[Idx];
		return Result;
	}

	FVec4 FVec4::operator*(float InValue) const
	{
		FVec4 Result;
		for (size_t Idx = 0; Idx < 4; Idx++)
			Result.Values[Idx] = Values[Idx] * InValue;
		return Result;
	}

	FVec4 FVec4::operator-() const
	{
		return FVec4(-X, -Y, -Z, -W);
	}

	float FVec4::Dot(const FVec4& InOther) const
	{
		float DotProduct = 0.0f;
		for (size_t Idx = 0; Idx < 4; Idx++)
			DotProduct += Values[Idx] * InOther.Values[Idx];
		return DotProduct;
	}

	float FVec4::LengthSq() const
	{
		return Dot(*this);
	}

	float FVec4::Length() const
	{
		return sqrt(LengthSq());
	}

	void FVec4::Normalize()
	{
		float Len = Length();

		if (Len == 0.0f)
			return;

		float InvLength = 1.0f / Len;
		for (size_t Idx = 0; Idx < 4; Idx++)
			Values[Idx] *= InvLength;
	}

	FVec4 FVec4::Normalized() const
	{
		FVec4 Result = *this;
		Result.Normalize();
		return Result;
	}

	float FVec4::operator[](size_t InIndex) const
	{
		return Values[InIndex];
	}

	FVec4 FVec4::Max(const FVec4& InVec0, const FVec4& InVec1)
	{
		FVec4 Result;
		for (size_t Idx = 0; Idx < 4; Idx++)
			Result.Values[Idx] = std::max(InVec0.Values[Idx], InVec1.Values[Idx]);
		return Result;
	}

	FVec4 FVec4::Min(const FVec4& InVec0, const FVec4& InVec1)
	{
		FVec4 Result;
		for (size_t Idx = 0; Idx < 4; Idx++)
			Result.Values[Idx] = std::min(InVec0.Values[Idx], InVec1.Values[Idx]);
		return Result;
	}

	FVec4 FVec4::Zero() { return FVec4(0.0f); }

}
