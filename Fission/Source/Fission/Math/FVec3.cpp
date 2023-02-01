#include "FissionPCH.hpp"
#include "FVec3.hpp"
#include "FVec4.hpp"
#include "Math.hpp"

#include "Fission/Core/Assert.hpp"

namespace Fission::Math {

	FVec3::FVec3(float InScalar)
	{
		for (size_t i = 0; i < 3; i++)
			Values[i] = InScalar;
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
		for (size_t i = 0; i < 3; i++)
			Values[i] += InOther.Values[i];
		return *this;
	}

	FVec3& FVec3::operator+=(float InValue)
	{
		for (size_t i = 0; i < 3; i++)
			Values[i] += InValue;
		return *this;
	}

	FVec3& FVec3::operator-=(const FVec3& InOther)
	{
		for (size_t i = 0; i < 3; i++)
			Values[i] -= InOther.Values[i];
		return *this;
	}

	FVec3& FVec3::operator-=(float InValue)
	{
		for (size_t i = 0; i < 3; i++)
			Values[i] -= InValue;
		return *this;
	}

	FVec3& FVec3::operator/=(const FVec3& InOther)
	{
		for (size_t i = 0; i < 3; i++)
			Values[i] /= InOther.Values[i];
		return *this;
	}

	FVec3& FVec3::operator/=(float InValue)
	{
		for (size_t i = 0; i < 3; i++)
			Values[i] /= InValue;
		return *this;
	}

	FVec3& FVec3::operator*=(const FVec3& InOther)
	{
		for (size_t i = 0; i < 3; i++)
			Values[i] *= InOther.Values[i];
		return *this;
	}

	FVec3& FVec3::operator*=(float InValue)
	{
		for (size_t i = 0; i < 3; i++)
			Values[i] *= InValue;
		return *this;
	}

	FVec3 FVec3::operator+(const FVec3& InOther) const
	{
		FVec3 result;
		for (size_t i = 0; i < 3; i++)
			result.Values[i] = Values[i] + InOther.Values[i];
		return result;
	}

	FVec3 FVec3::operator+(float InValue) const
	{
		FVec3 result;
		for (size_t i = 0; i < 3; i++)
			result.Values[i] = Values[i] + InValue;
		return result;

	}

	FVec3 FVec3::operator-(const FVec3& InOther) const
	{
		FVec3 result;
		for (size_t i = 0; i < 3; i++)
			result.Values[i] = Values[i] - InOther.Values[i];
		return result;
	}

	FVec3 FVec3::operator-(float InValue) const
	{
		FVec3 result;
		for (size_t i = 0; i < 3; i++)
			result.Values[i] = Values[i] - InValue;
		return result;
	}

	FVec3 FVec3::operator/(const FVec3& InOther) const
	{
		FVec3 result;
		for (size_t i = 0; i < 3; i++)
			result.Values[i] = Values[i] / InOther.Values[i];
		return result;
	}

	FVec3 FVec3::operator/(float InValue) const
	{
		FVec3 result;
		for (size_t i = 0; i < 3; i++)
			result.Values[i] = Values[i] / InValue;
		return result;
	}

	FVec3 FVec3::operator*(const FVec3& InOther) const
	{
		FVec3 result;
		for (size_t i = 0; i < 3; i++)
			result.Values[i] = Values[i] * InOther.Values[i];
		return result;
	}

	FVec3 FVec3::operator*(float InValue) const
	{
		FVec3 result;
		for (size_t i = 0; i < 3; i++)
			result.Values[i] = Values[i] * InValue;
		return result;
	}

	FVec3 FVec3::operator-() const
	{
		return FVec3(-X, -Y, -Z);
	}

	float FVec3::Dot(const FVec3& InOther) const
	{
		float dotProduct = 0.0f;
		for (size_t i = 0; i < 3; i++)
			dotProduct += Values[i] * InOther.Values[i];
		return dotProduct;
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
		float length = Length();

		if (IsNearZero(length))
			return;

		float invLength = 1.0f / length;
		for (size_t i = 0; i < 3; i++)
			Values[i] *= invLength;
	}

	FVec3 FVec3::Normalized() const
	{
		FVec3 result = *this;
		result.Normalize();
		return result;
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
		FVec3 result;
		for (size_t i = 0; i < 3; i++)
			result.Values[i] = std::max(InVec0.Values[i], InVec1.Values[i]);
		return result;
	}

	FVec3 FVec3::Min(const FVec3& InVec0, const FVec3& InVec1)
	{
		FVec3 result;
		for (size_t i = 0; i < 3; i++)
			result.Values[i] = std::min(InVec0.Values[i], InVec1.Values[i]);
		return result;
	}

	FVec3 FVec3::Zero() { return FVec3(0.0f); }

	bool FVec3::IsValid() const
	{
		for (size_t i = 0; i < 3; i++)
		{
			if (Values[i] != Values[i])
				return false;
		}

		return true;
	}

}
