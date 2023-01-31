#pragma once

#include "FVec3.hpp"
#include "FVec4.hpp"
#include "FQuat.hpp"

namespace Fission::Math {

	struct Mat4x4
	{
		FVec4 Columns[4];

		Mat4x4() = default;
		Mat4x4(const Mat4x4&) = default;

		Mat4x4(float InDiagonal);
		Mat4x4(const FVec4& InColumn0, const FVec4& InColumn1, const FVec4& InColumn2, const FVec4& InColumn3);

		FVec4& operator[](size_t InIndex) { return Columns[InIndex]; }
		const FVec4& operator[](size_t InIndex) const { return Columns[InIndex]; }

		Mat4x4 operator*(const Mat4x4& InOther) const;
		Mat4x4 operator*(float InOther) const;

		void Inverse();
		Mat4x4 Inversed() const;
		//Mat4x4 Transposed() const;

		static Mat4x4 CreateProjection(float InFieldOfView, float InWidth, float InHeight, float InNear, float InFar);
		static Mat4x4 CreateTranslation(const FVec3& InTranslation);
		
		static Mat4x4 LookAt(const FVec3& InEye, const FVec3& InCenter, const FVec3& InUp);

		static Mat4x4 FromQuat(const FQuat& InRotation);

	};

}
