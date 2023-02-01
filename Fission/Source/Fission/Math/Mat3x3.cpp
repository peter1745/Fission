#include "FissionPCH.hpp"
#include "Mat3x3.hpp"
#include "Math.hpp"

namespace Fission::Math {

	Mat3x3::Mat3x3(float InDiagonal)
	{
		memset(Columns, 0, sizeof(Columns));

		Columns[0][0] = InDiagonal;
		Columns[1][1] = InDiagonal;
		Columns[2][2] = InDiagonal;
	}

	Mat3x3::Mat3x3(const FVec3& InColumn0, const FVec3& InColumn1, const FVec3& InColumn2)
	{
		Columns[0] = InColumn0;
		Columns[1] = InColumn1;
		Columns[2] = InColumn2;
	}

	Mat3x3 Mat3x3::operator*(const Mat3x3& InOther) const
	{
		Mat3x3 result;

		for (size_t Idx = 0; Idx < 3; Idx++)
			result.Columns[Idx] = Columns[0] * InOther.Columns[Idx][0] + Columns[1] * InOther.Columns[Idx][1] + Columns[2] * InOther.Columns[Idx][2];

		return result;
	}

	FVec3 Mat3x3::operator*(const FVec3& InOther) const
	{
		return FVec3(
		    Columns[0][0] * InOther[0] + Columns[1][0] * InOther[1] + Columns[2][0] * InOther[2],
		    Columns[0][1] * InOther[0] + Columns[1][1] * InOther[1] + Columns[2][1] * InOther[2],
		    Columns[0][2] * InOther[0] + Columns[1][2] * InOther[1] + Columns[2][2] * InOther[2]);
	}

	Mat3x3 Mat3x3::operator*(float InOther) const
	{
		Mat3x3 result;
		FVec3 scalar(InOther);

		for (size_t Idx = 0; Idx < 3; Idx++)
			result.Columns[Idx] = Columns[Idx] * scalar;

		return result;
	}

	Mat3x3 Mat3x3::Inversed() const
	{
		float det = Columns[0].Dot(Columns[1].Cross(Columns[2]));

		if (IsNearZero(det))
			return *this;

		FVec3 column0((Columns[1][1] * Columns[2][2] - Columns[1][2] * Columns[2][1]) / det,
					  (Columns[1][2] * Columns[2][0] - Columns[1][0] * Columns[2][2]) / det,
					  (Columns[1][0] * Columns[2][1] - Columns[1][1] * Columns[2][0]) / det);

		FVec3 column1((Columns[0][2] * Columns[2][1] - Columns[0][1] * Columns[2][2]) / det,
					  (Columns[0][0] * Columns[2][2] - Columns[0][2] * Columns[2][0]) / det,
					  (Columns[0][1] * Columns[2][0] - Columns[0][0] * Columns[2][1]) / det);

		FVec3 column2((Columns[0][1] * Columns[1][2] - Columns[0][2] * Columns[1][1]) / det,
					  (Columns[0][2] * Columns[1][0] - Columns[0][0] * Columns[1][2]) / det,
					  (Columns[0][0] * Columns[1][1] - Columns[0][1] * Columns[1][0]) / det);

		return Mat3x3(column0, column1, column2);
	}

	Mat3x3 Mat3x3::FromQuat(const FQuat& InQuat)
	{
		Mat3x3 result;

		float x = InQuat.Value.X;
		float y = InQuat.Value.Y;
		float z = InQuat.Value.Z;
		float w = InQuat.Value.W;

		float tx = x + x;
		float ty = y + y;
		float tz = z + z;

		float xx = tx * x;
		float yy = ty * y;
		float zz = tz * z;
		float xy = tx * y;
		float xz = tx * z;
		float xw = tx * w;
		float yz = ty * z;
		float yw = ty * w;
		float zw = tz * w;

		result[0] = FVec3((1.0f - yy) - zz, xy + zw, xz - yw);
		result[1] = FVec3(xz - zw, (1.0f - zz) - xx, yz + xw);
		result[2] = FVec3(xz + yw, yz - xw, (1.0f - xx) - yy);

		return result;
	}

	Mat3x3 Mat3x3::Transposed() const
	{
		Mat3x3 result;

		result[0][0] = Columns[0][0];
		result[0][1] = Columns[1][0];
		result[0][2] = Columns[2][0];

		result[1][0] = Columns[0][1];
		result[1][1] = Columns[1][1];
		result[1][2] = Columns[2][1];

		result[2][0] = Columns[0][2];
		result[2][1] = Columns[1][2];
		result[2][2] = Columns[2][2];

		return result;
	}

}
