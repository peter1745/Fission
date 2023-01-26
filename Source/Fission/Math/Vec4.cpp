#include "FissionPCH.hpp"
#include "Vec4.hpp"

namespace Fission::Math {

	Vec4::Vec4(float InScalar)
	{
#ifdef FISSION_USE_SSE
		Value = _mm_set1_ps(InScalar);
#else
		for (size_t Idx = 0; Idx < 4; Idx++)
			Floats[Idx] = InScalar;
#endif
	}

	Vec4::Vec4(float InX, float InY, float InZ, float InW)
	{
#ifdef FISSION_USE_SSE
		Value = _mm_setr_ps(InX, InY, InZ, InW);
#else
		Floats[0] = InX;
		Floats[1] = InY;
		Floats[2] = InZ;
		Floats[3] = InW;
#endif
	}

	float Vec4::Dot(const Vec4& InOther) const
	{
#ifdef FISSION_USE_SSE
		return _mm_cvtss_f32(_mm_dp_ps(Value, InOther.Value, 0xFF));
#else
		float DotProduct = 0.0f;
		for (size_t Idx = 0; Idx < 3; Idx++)
			DotProduct += Floats[Idx] * InOther.Floats[Idx];
		return DotProduct;
#endif
	}

	float Vec4::Length() const
	{
#ifdef FISSION_USE_SSE
		return _mm_cvtss_f32(_mm_sqrt_ps(_mm_dp_ps(Value, Value, 0xFF)));
#else
		return sqrt(LengthSq());
#endif
	}

	void Vec4::Normalize()
	{
#ifdef FISSION_USE_SSE
		__m128 InvLength = _mm_div_ps(_mm_set1_ps(1.0f) , _mm_sqrt_ps(_mm_dp_ps(Value, Value, 0xFF)));
		Value = _mm_mul_ps(Value, InvLength);
#else
		float InvLength = 1.0f / Length();
		for (size_t Idx = 0; Idx < 4; Idx++)
			Floats[Idx] *= InvLength;
#endif
	}

	Vec4 Vec4::Normalized()
	{
		Vec4 Result(Value);
		Result.Normalize();
		return Result;
	}

	bool Vec4::operator==(const Vec4& InOther) const
	{
#ifdef FISSION_USE_SSE
		return _mm_movemask_ps(_mm_cmp_ps(Value, InOther.Value, _CMP_EQ_OS)) == 0xF;
#else
		for (size_t Idx = 0; Idx < 4; Idx++)
		{
			if (Floats[Idx] != InOther.Floats[Idx])
				return false;
		}

		return true;
#endif
	}

	bool Vec4::operator!=(const Vec4& InOther) const { return !(*this == InOther); }

	Vec4 Vec4::operator+(const Vec4& InOther) const
	{
#ifdef FISSION_USE_SSE
		return _mm_add_ps(Value, InOther.Value);
#else
		Vec4 Result;
		for (size_t Idx = 0; Idx < 4; Idx++)
			Result.Floats[Idx] = Floats[Idx] + InOther.Floats[Idx];
		return Result;
#endif
	}

	Vec4 Vec4::operator+(float InValue) const
	{
#ifdef FISSION_USE_SSE
		return _mm_add_ps(Value, _mm_set1_ps(InValue));
#else
		return Vec4(
		    Floats[0] + InValue,
		    Floats[1] + InValue,
		    Floats[2] + InValue
		);
#endif
	}

	Vec4 Vec4::operator-(const Vec4& InOther) const
	{
#ifdef FISSION_USE_SSE
		return _mm_sub_ps(Value, InOther.Value);
#else
		Vec4 Result;
		for (size_t Idx = 0; Idx < 4; Idx++)
			Result.Floats[Idx] = Floats[Idx] - InOther.Floats[Idx];
		return Result;
#endif
	}

	Vec4 Vec4::operator-(float InValue) const
	{
#ifdef FISSION_USE_SSE
		return _mm_sub_ps(Value, _mm_set1_ps(InValue));
#else
		return Vec4(
		    Floats[0] - InValue,
		    Floats[1] - InValue,
		    Floats[2] - InValue);
#endif
	}

	Vec4 Vec4::operator/(const Vec4& InOther) const
	{
#ifdef FISSION_USE_SSE
		return _mm_div_ps(Value, InOther.Value);
#else
		Vec4 Result;
		for (size_t Idx = 0; Idx < 4; Idx++)
			Result.Floats[Idx] = Floats[Idx] / InOther.Floats[Idx];
		return Result;
#endif
	}

	Vec4 Vec4::operator/(float InValue) const
	{
#ifdef FISSION_USE_SSE
		return _mm_div_ps(Value, _mm_set1_ps(InValue));
#else
		return Vec4(
		    Floats[0] / InValue,
		    Floats[1] / InValue,
		    Floats[2] / InValue);
#endif
	}

	Vec4 Vec4::operator*(const Vec4& InOther) const
	{
#ifdef FISSION_USE_SSE
		return _mm_mul_ps(Value, InOther.Value);
#else
		Vec4 Result;
		for (size_t Idx = 0; Idx < 4; Idx++)
			Result.Floats[Idx] = Floats[Idx] * InOther.Floats[Idx];
		return Result;
#endif
	}

	Vec4 Vec4::operator*(float InValue) const
	{
#ifdef FISSION_USE_SSE
		return _mm_mul_ps(Value, _mm_set1_ps(InValue));
#else
		return Vec4(
		    Floats[0] * InValue,
		    Floats[1] * InValue,
		    Floats[2] * InValue);
#endif
	}

	Vec4& Vec4::operator+=(const Vec4& InOther)
	{
#ifdef FISSION_USE_SSE
		Value = _mm_add_ps(Value, InOther.Value);
#else
		for (size_t Idx = 0; Idx < 4; Idx++)
			Floats[Idx] += InOther.Floats[Idx];
#endif

		return *this;
	}

	Vec4& Vec4::operator+=(float InValue)
	{
#ifdef FISSION_USE_SSE
		Value = _mm_add_ps(Value, _mm_set1_ps(InValue));
#else
		for (size_t Idx = 0; Idx < 4; Idx++)
			Floats[Idx] += InValue;
#endif

		return *this;
	}

	Vec4& Vec4::operator-=(const Vec4& InOther)
	{
#ifdef FISSION_USE_SSE
		Value = _mm_sub_ps(Value, InOther.Value);
#else
		for (size_t Idx = 0; Idx < 4; Idx++)
			Floats[Idx] -= InOther.Floats[Idx];
#endif

		return *this;
	}

	Vec4& Vec4::operator-=(float InValue)
	{
#ifdef FISSION_USE_SSE
		Value = _mm_sub_ps(Value, _mm_set1_ps(InValue));
#else
		for (size_t Idx = 0; Idx < 4; Idx++)
			Floats[Idx] -= InValue;
#endif

		return *this;
	}

	Vec4& Vec4::operator/=(const Vec4& InOther)
	{
#ifdef FISSION_USE_SSE
		Value = _mm_div_ps(Value, InOther.Value);
#else
		for (size_t Idx = 0; Idx < 4; Idx++)
			Floats[Idx] /= InOther.Floats[Idx];
#endif

		return *this;
	}

	Vec4& Vec4::operator/=(float InValue)
	{
#ifdef FISSION_USE_SSE
		Value = _mm_div_ps(Value, _mm_set1_ps(InValue));
#else
		for (size_t Idx = 0; Idx < 4; Idx++)
			Floats[Idx] /= InValue;
#endif

		return *this;
	}

	Vec4& Vec4::operator*=(const Vec4& InOther)
	{
#ifdef FISSION_USE_SSE
		Value = _mm_mul_ps(Value, InOther.Value);
#else
		for (size_t Idx = 0; Idx < 4; Idx++)
			Floats[Idx] *= InOther.Floats[Idx];
#endif

		return *this;
	}

	Vec4& Vec4::operator*=(float InValue)
	{
#ifdef FISSION_USE_SSE
		Value = _mm_mul_ps(Value, _mm_set1_ps(InValue));
#else
		for (size_t Idx = 0; Idx < 4; Idx++)
			Floats[Idx] *= InValue;
#endif

		return *this;
	}

	Vec4 Vec4::operator-() const
	{
#ifdef FISSION_USE_SSE
		return _mm_sub_ps(_mm_setzero_ps(), Value);
#else
		return Vec4(-Floats[0], -Floats[1], -Floats[2]);
#endif
	}

	Vec4 Vec4::Min(const Vec4& InVec0, const Vec4& InVec1)
	{
#ifdef FISSION_USE_SSE
		return _mm_min_ps(InVec0.Value, InVec1.Value);
#else
		return Vec4(
		    std::min(InVec0.Floats[0], InVec1.Floats[0]),
		    std::min(InVec0.Floats[1], InVec1.Floats[1]),
		    std::min(InVec0.Floats[2], InVec1.Floats[2])
		);
#endif
	}

	Vec4 Vec4::Max(const Vec4& InVec0, const Vec4& InVec1)
	{
#ifdef FISSION_USE_SSE
		return _mm_max_ps(InVec0.Value, InVec1.Value);
#else
		return Vec4(
		    std::max(InVec0.Floats[0], InVec1.Floats[0]),
		    std::max(InVec0.Floats[1], InVec1.Floats[1]),
		    std::max(InVec0.Floats[2], InVec1.Floats[2]));
#endif
	}

	Vec4 Vec4::Zero()
	{
#ifdef FISSION_USE_SSE
		return _mm_setzero_ps();
#else
		return Vec4(0.0f);
#endif
	}

}
