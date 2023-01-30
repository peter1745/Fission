#include "FissionPCH.hpp"
#include "FVec4.hpp"

namespace Fission::Math {

	FVec4::FVec4(float InScalar)
	{
		for (size_t i = 0; i < 4; i++)
			Values[i] = InScalar;
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
		for (size_t i = 0; i < 4; i++)
			Values[i] += InOther.Values[i];
		return *this;
	}

	FVec4& FVec4::operator+=(float InValue)
	{
		for (size_t i = 0; i < 4; i++)
			Values[i] += InValue;
		return *this;
	}

	FVec4& FVec4::operator-=(const FVec4& InOther)
	{
		for (size_t i = 0; i < 4; i++)
			Values[i] -= InOther.Values[i];
		return *this;
	}

	FVec4& FVec4::operator-=(float InValue)
	{
		for (size_t i = 0; i < 4; i++)
			Values[i] -= InValue;
		return *this;
	}

	FVec4& FVec4::operator/=(const FVec4& InOther)
	{
		for (size_t i = 0; i < 4; i++)
			Values[i] /= InOther.Values[i];
		return *this;
	}

	FVec4& FVec4::operator/=(float InValue)
	{
		for (size_t i = 0; i < 4; i++)
			Values[i] /= InValue;
		return *this;
	}

	FVec4& FVec4::operator*=(const FVec4& InOther)
	{
		for (size_t i = 0; i < 4; i++)
			Values[i] *= InOther.Values[i];
		return *this;
	}

	FVec4& FVec4::operator*=(float InValue)
	{
		for (size_t i = 0; i < 4; i++)
			Values[i] *= InValue;
		return *this;
	}

	FVec4 FVec4::operator+(const FVec4& InOther) const
	{
		FVec4 result;
		for (size_t i = 0; i < 4; i++)
			result.Values[i] = Values[i] + InOther.Values[i];
		return result;
	}

	FVec4 FVec4::operator+(float InValue) const
	{
		FVec4 result;
		for (size_t i = 0; i < 4; i++)
			result.Values[i] = Values[i] + InValue;
		return result;

	}

	FVec4 FVec4::operator-(const FVec4& InOther) const
	{
		FVec4 result;
		for (size_t i = 0; i < 4; i++)
			result.Values[i] = Values[i] - InOther.Values[i];
		return result;
	}

	FVec4 FVec4::operator-(float InValue) const
	{
		FVec4 result;
		for (size_t i = 0; i < 4; i++)
			result.Values[i] = Values[i] - InValue;
		return result;
	}

	FVec4 FVec4::operator/(const FVec4& InOther) const
	{
		FVec4 result;
		for (size_t i = 0; i < 4; i++)
			result.Values[i] = Values[i] / InOther.Values[i];
		return result;
	}

	FVec4 FVec4::operator/(float InValue) const
	{
		FVec4 result;
		for (size_t i = 0; i < 4; i++)
			result.Values[i] = Values[i] / InValue;
		return result;
	}

	FVec4 FVec4::operator*(const FVec4& InOther) const
	{
		FVec4 result;
		for (size_t i = 0; i < 4; i++)
			result.Values[i] = Values[i] * InOther.Values[i];
		return result;
	}

	FVec4 FVec4::operator*(float InValue) const
	{
		FVec4 result;
		for (size_t i = 0; i < 4; i++)
			result.Values[i] = Values[i] * InValue;
		return result;
	}

	FVec4 FVec4::operator-() const
	{
		return FVec4(-X, -Y, -Z, -W);
	}

	float FVec4::Dot(const FVec4& InOther) const
	{
		float dotProduct = 0.0f;
		for (size_t i = 0; i < 4; i++)
			dotProduct += Values[i] * InOther.Values[i];
		return dotProduct;
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
		float length = Length();

		if (length == 0.0f)
			return;

		float invLength = 1.0f / length;
		for (size_t i = 0; i < 4; i++)
			Values[i] *= invLength;
	}

	FVec4 FVec4::Normalized() const
	{
		FVec4 result = *this;
		result.Normalize();
		return result;
	}

	float FVec4::operator[](size_t InIndex) const
	{
		return Values[InIndex];
	}

	FVec4 FVec4::Max(const FVec4& InVec0, const FVec4& InVec1)
	{
		FVec4 result;
		for (size_t i = 0; i < 4; i++)
			result.Values[i] = std::max(InVec0.Values[i], InVec1.Values[i]);
		return result;
	}

	FVec4 FVec4::Min(const FVec4& InVec0, const FVec4& InVec1)
	{
		FVec4 result;
		for (size_t i = 0; i < 4; i++)
			result.Values[i] = std::min(InVec0.Values[i], InVec1.Values[i]);
		return result;
	}

	FVec4 FVec4::Zero() { return FVec4(0.0f); }

}
