#pragma once

#include "FVec3.hpp"
#include "FQuat.hpp"

namespace Fission::Math {

	struct Mat3x3
	{
		FVec3 Columns[3];

		Mat3x3() = default;
		Mat3x3(const Mat3x3&) = default;

		Mat3x3(float InDiagonal);
		Mat3x3(const FVec3& InColumn0, const FVec3& InColumn1, const FVec3& InColumn2);

		FVec3& operator[](size_t InIndex) { return Columns[InIndex]; }
		const FVec3& operator[](size_t InIndex) const { return Columns[InIndex]; }

		Mat3x3 operator*(const Mat3x3& InOther) const;
		Mat3x3 operator*(float InOther) const;

		FVec3 operator*(const FVec3& InOther) const;

		Mat3x3 Inversed() const;
		Mat3x3 Transposed() const;

		static Mat3x3 FromQuat(const FQuat& InQuat);

	};

}
