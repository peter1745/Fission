#pragma once

#include "Fission/Core/Core.hpp"

namespace Fission::Math {

	struct alignas(16) Vec4
	{
#ifdef FISSION_USE_SSE
		using Type = __m128;
#else
		using Type = struct { float Data[4]; };
#endif

		union
		{
			Type Value;
			float Floats[4];
		};

		Vec4() = default;
		Vec4(const Vec4&) = default;

		Vec4(float InScalar);
		Vec4(float InX, float InY, float InZ, float InW);
		Vec4(Type InValue)
		    : Value(InValue) {}

		#ifdef FISSION_USE_SSE
		float GetX() const
		{
			return _mm_cvtss_f32(Value);
		}
		float GetY() const { return Floats[1]; }
		float GetZ() const { return Floats[2]; }
		float GetW() const { return Floats[3]; }
#else
		float GetX() const
		{
			return Floats[0];
		}
		float GetY() const { return Floats[1]; }
		float GetZ() const { return Floats[2]; }
		float GetW() const { return Floats[3]; }
#endif

		float Dot(const Vec4& InOther) const;

		float LengthSq() const { return Dot(*this); }
		float Length() const;

		void Normalize();
		Vec4 Normalized();

		float operator[](size_t InIdx) const { return Floats[InIdx]; }

		bool operator==(const Vec4& InOther) const;
		bool operator!=(const Vec4& InOther) const;

		Vec4 operator+(const Vec4& InOther) const;
		Vec4 operator+(float InValue) const;

		Vec4 operator-(const Vec4& InOther) const;
		Vec4 operator-(float InValue) const;

		Vec4 operator/(const Vec4& InOther) const;
		Vec4 operator/(float InValue) const;

		Vec4 operator*(const Vec4& InOther) const;
		Vec4 operator*(float InValue) const;

		Vec4& operator+=(const Vec4& InOther);
		Vec4& operator+=(float InValue);

		Vec4& operator-=(const Vec4& InOther);
		Vec4& operator-=(float InValue);

		Vec4& operator/=(const Vec4& InOther);
		Vec4& operator/=(float InValue);

		Vec4& operator*=(const Vec4& InOther);
		Vec4& operator*=(float InValue);

		Vec4 operator-() const;

		static Vec4 Min(const Vec4& InVec0, const Vec4& InVec1);
		static Vec4 Max(const Vec4& InVec0, const Vec4& InVec1);

		static Vec4 Zero();

	};

}
