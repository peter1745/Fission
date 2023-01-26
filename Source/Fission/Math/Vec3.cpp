#include "FissionPCH.hpp"
#include "Vec3.hpp"

namespace Fission::Math {

	Vec3::Vec3(float InScalar)
	{
#ifdef FISSION_USE_SSE
		Value = _mm_set1_ps(InScalar);
#else
		for (size_t Idx = 0; Idx < 4; Idx++)
			Floats[Idx] = InScalar;
#endif
	}

	Vec3::Vec3(float InX, float InY, float InZ)
	{
#ifdef FISSION_USE_SSE
		Value = _mm_setr_ps(InX, InY, InZ, InZ);
#else
		Floats[0] = InX;
		Floats[1] = InY;
		Floats[2] = InZ;
		Floats[3] = InZ;
#endif
	}

	float Vec3::Dot(const Vec3& InOther) const
	{
#ifdef FISSION_USE_SSE
		return _mm_cvtss_f32(_mm_dp_ps(Value, InOther.Value, 0x7F));
#else
		float DotProduct = 0.0f;
		for (size_t Idx = 0; Idx < 3; Idx++)
			DotProduct += Floats[Idx] * InOther.Floats[Idx];
		return DotProduct;
#endif
	}

	float Vec3::Length() const
	{
#ifdef FISSION_USE_SSE
		return _mm_cvtss_f32(_mm_sqrt_ps(_mm_dp_ps(Value, Value, 0x7F)));
#else
		return sqrt(LengthSq());
#endif
	}

	bool Vec3::operator==(const Vec3& InOther) const
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

	bool Vec3::operator!=(const Vec3& InOther) const { return !(*this == InOther); }

	Vec3 Vec3::operator+(const Vec3& InOther) const
	{
#ifdef FISSION_USE_SSE
		return _mm_add_ps(Value, InOther.Value);
#else
		Vec3 Result;
		for (size_t Idx = 0; Idx < 4; Idx++)
			Result.Floats[Idx] = Floats[Idx] + InOther.Floats[Idx];
		return Result;
#endif
	}

	Vec3 Vec3::operator+(float InValue) const
	{
#ifdef FISSION_USE_SSE
		return _mm_add_ps(Value, _mm_set1_ps(InValue));
#else
		return Vec3(
		    Floats[0] + InValue,
		    Floats[1] + InValue,
		    Floats[2] + InValue
		);
#endif
	}

	Vec3 Vec3::operator-(const Vec3& InOther) const
	{
#ifdef FISSION_USE_SSE
		return _mm_sub_ps(Value, InOther.Value);
#else
		Vec3 Result;
		for (size_t Idx = 0; Idx < 4; Idx++)
			Result.Floats[Idx] = Floats[Idx] - InOther.Floats[Idx];
		return Result;
#endif
	}

	Vec3 Vec3::operator-(float InValue) const
	{
#ifdef FISSION_USE_SSE
		return _mm_sub_ps(Value, _mm_set1_ps(InValue));
#else
		return Vec3(
		    Floats[0] - InValue,
		    Floats[1] - InValue,
		    Floats[2] - InValue);
#endif
	}

	Vec3 Vec3::operator/(const Vec3& InOther) const
	{
#ifdef FISSION_USE_SSE
		return _mm_div_ps(Value, InOther.Value);
#else
		Vec3 Result;
		for (size_t Idx = 0; Idx < 4; Idx++)
			Result.Floats[Idx] = Floats[Idx] / InOther.Floats[Idx];
		return Result;
#endif
	}

	Vec3 Vec3::operator/(float InValue) const
	{
#ifdef FISSION_USE_SSE
		return _mm_div_ps(Value, _mm_set1_ps(InValue));
#else
		return Vec3(
		    Floats[0] / InValue,
		    Floats[1] / InValue,
		    Floats[2] / InValue);
#endif
	}

	Vec3 Vec3::operator*(const Vec3& InOther) const
	{
#ifdef FISSION_USE_SSE
		return _mm_mul_ps(Value, InOther.Value);
#else
		Vec3 Result;
		for (size_t Idx = 0; Idx < 4; Idx++)
			Result.Floats[Idx] = Floats[Idx] * InOther.Floats[Idx];
		return Result;
#endif
	}

	Vec3 Vec3::operator*(float InValue) const
	{
#ifdef FISSION_USE_SSE
		return _mm_mul_ps(Value, _mm_set1_ps(InValue));
#else
		return Vec3(
		    Floats[0] * InValue,
		    Floats[1] * InValue,
		    Floats[2] * InValue);
#endif
	}

	Vec3& Vec3::operator+=(const Vec3& InOther)
	{
#ifdef FISSION_USE_SSE
		Value = _mm_add_ps(Value, InOther.Value);
#else
		for (size_t Idx = 0; Idx < 4; Idx++)
			Floats[Idx] += InOther.Floats[Idx];
#endif

		return *this;
	}

	Vec3& Vec3::operator+=(float InValue)
	{
#ifdef FISSION_USE_SSE
		Value = _mm_add_ps(Value, _mm_set1_ps(InValue));
#else
		for (size_t Idx = 0; Idx < 4; Idx++)
			Floats[Idx] += InValue;
#endif

		return *this;
	}

	Vec3& Vec3::operator-=(const Vec3& InOther)
	{
#ifdef FISSION_USE_SSE
		Value = _mm_sub_ps(Value, InOther.Value);
#else
		for (size_t Idx = 0; Idx < 4; Idx++)
			Floats[Idx] -= InOther.Floats[Idx];
#endif

		return *this;
	}

	Vec3& Vec3::operator-=(float InValue)
	{
#ifdef FISSION_USE_SSE
		Value = _mm_sub_ps(Value, _mm_set1_ps(InValue));
#else
		for (size_t Idx = 0; Idx < 4; Idx++)
			Floats[Idx] -= InValue;
#endif

		return *this;
	}

	Vec3& Vec3::operator/=(const Vec3& InOther)
	{
#ifdef FISSION_USE_SSE
		Value = _mm_div_ps(Value, InOther.Value);
#else
		for (size_t Idx = 0; Idx < 4; Idx++)
			Floats[Idx] /= InOther.Floats[Idx];
#endif

		return *this;
	}

	Vec3& Vec3::operator/=(float InValue)
	{
#ifdef FISSION_USE_SSE
		Value = _mm_div_ps(Value, _mm_set1_ps(InValue));
#else
		for (size_t Idx = 0; Idx < 4; Idx++)
			Floats[Idx] /= InValue;
#endif

		return *this;
	}

	Vec3& Vec3::operator*=(const Vec3& InOther)
	{
#ifdef FISSION_USE_SSE
		Value = _mm_mul_ps(Value, InOther.Value);
#else
		for (size_t Idx = 0; Idx < 4; Idx++)
			Floats[Idx] *= InOther.Floats[Idx];
#endif

		return *this;
	}

	Vec3& Vec3::operator*=(float InValue)
	{
#ifdef FISSION_USE_SSE
		Value = _mm_mul_ps(Value, _mm_set1_ps(InValue));
#else
		for (size_t Idx = 0; Idx < 4; Idx++)
			Floats[Idx] *= InValue;
#endif

		return *this;
	}

	Vec3 Vec3::operator-() const
	{
#ifdef FISSION_USE_SSE
		return _mm_sub_ps(_mm_setzero_ps(), Value);
#else
		return Vec3(-Floats[0], -Floats[1], -Floats[2]);
#endif
	}

	Vec3 Vec3::Min(const Vec3& InVec0, const Vec3& InVec1)
	{
#ifdef FISSION_USE_SSE
		return _mm_min_ps(InVec0.Value, InVec1.Value);
#else
		return Vec3(
		    std::min(InVec0.Floats[0], InVec1.Floats[0]),
		    std::min(InVec0.Floats[1], InVec1.Floats[1]),
		    std::min(InVec0.Floats[2], InVec1.Floats[2])
		);
#endif
	}

	Vec3 Vec3::Max(const Vec3& InVec0, const Vec3& InVec1)
	{
#ifdef FISSION_USE_SSE
		return _mm_max_ps(InVec0.Value, InVec1.Value);
#else
		return Vec3(
		    std::max(InVec0.Floats[0], InVec1.Floats[0]),
		    std::max(InVec0.Floats[1], InVec1.Floats[1]),
		    std::max(InVec0.Floats[2], InVec1.Floats[2]));
#endif
	}

	Vec3 Vec3::Zero()
	{
#ifdef FISSION_USE_SSE
		return _mm_setzero_ps();
#else
		return Vec3(0.0f);
#endif
	}

}
