#include "FissionPCH.hpp"
#include "Mat3x3.hpp"

namespace Fission::Math {

	Mat3x3::Mat3x3(float InDiagonal)
	{
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
		Mat3x3 Result;

		for (size_t Idx = 0; Idx < 3; Idx++)
			Result.Columns[Idx] = Columns[0] * InOther.Columns[Idx][0] + Columns[1] * InOther.Columns[Idx][1] + Columns[2] * InOther.Columns[Idx][2];

		return Result;
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
		Mat3x3 Result;
		FVec3 Scalar(InOther);

		for (size_t Idx = 0; Idx < 3; Idx++)
			Result.Columns[Idx] = Columns[Idx] * Scalar;

		return Result;
	}

	Mat3x3 Mat3x3::Inversed() const
	{
		float OneOverDeterminant = 1.0f / (Columns[0][0] * (Columns[1][1] * Columns[2][2] - Columns[2][1] * Columns[1][2]) - Columns[1][0] * (Columns[0][1] * Columns[2][2] - Columns[2][1] * Columns[0][2]) + Columns[2][0] * (Columns[0][1] * Columns[1][2] - Columns[1][1] * Columns[0][2]));

		Mat3x3 Inverse;
		Inverse[0][0] = +(Columns[1][1] * Columns[2][2] - Columns[2][1] * Columns[1][2]) * OneOverDeterminant;
		Inverse[1][0] = -(Columns[1][0] * Columns[2][2] - Columns[2][0] * Columns[1][2]) * OneOverDeterminant;
		Inverse[2][0] = +(Columns[1][0] * Columns[2][1] - Columns[2][0] * Columns[1][1]) * OneOverDeterminant;
		Inverse[0][1] = -(Columns[0][1] * Columns[2][2] - Columns[2][1] * Columns[0][2]) * OneOverDeterminant;
		Inverse[1][1] = +(Columns[0][0] * Columns[2][2] - Columns[2][0] * Columns[0][2]) * OneOverDeterminant;
		Inverse[2][1] = -(Columns[0][0] * Columns[2][1] - Columns[2][0] * Columns[0][1]) * OneOverDeterminant;
		Inverse[0][2] = +(Columns[0][1] * Columns[1][2] - Columns[1][1] * Columns[0][2]) * OneOverDeterminant;
		Inverse[1][2] = -(Columns[0][0] * Columns[1][2] - Columns[1][0] * Columns[0][2]) * OneOverDeterminant;
		Inverse[2][2] = +(Columns[0][0] * Columns[1][1] - Columns[1][0] * Columns[0][1]) * OneOverDeterminant;

		return Inverse;
	}

	Mat3x3 Mat3x3::FromQuat(const FQuat& InQuat)
	{
		Mat3x3 Result;

		float X = InQuat.Value.X;
		float Y = InQuat.Value.Y;
		float Z = InQuat.Value.Z;
		float W = InQuat.Value.W;

		float TX = X + X;
		float TY = Y + Y;
		float TZ = Z + Z;

		float XX = TX * X;
		float YY = TY * Y;
		float ZZ = TZ * Z;
		float XY = TX * Y;
		float XZ = TX * Z;
		float XW = TX * W;
		float YZ = TY * Z;
		float YW = TY * W;
		float ZW = TZ * W;

		Result[0] = FVec3((1.0f - YY) - ZZ, XY + ZW, XZ - YW);
		Result[1] = FVec3(XZ - ZW, (1.0f - ZZ) - XX, YZ + XW);
		Result[2] = FVec3(XZ + YW, YZ - XW, (1.0f - XX) - YY);

		return Result;
	}

	Mat3x3 Mat3x3::Transposed() const
	{
		Mat3x3 Result;

		Result[0][0] = Columns[0][0];
		Result[0][1] = Columns[1][0];
		Result[0][2] = Columns[2][0];

		Result[1][0] = Columns[0][1];
		Result[1][1] = Columns[1][1];
		Result[1][2] = Columns[2][1];

		Result[2][0] = Columns[0][2];
		Result[2][1] = Columns[1][2];
		Result[2][2] = Columns[2][2];

		return Result;
	}

}
