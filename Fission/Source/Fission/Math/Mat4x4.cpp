#include "FissionPCH.hpp"
#include "Mat4x4.hpp"
#include "Math.hpp"

namespace Fission::Math {

	Mat4x4::Mat4x4(float InDiagonal)
	{
		memset(Columns, 0, sizeof(Columns));

		for (size_t i = 0; i < 4; i++)
			Columns[i][i] = InDiagonal;
	}

	Mat4x4::Mat4x4(const FVec4& InColumn0, const FVec4& InColumn1, const FVec4& InColumn2, const FVec4& InColumn3)
	{
		Columns[0] = InColumn0;
		Columns[1] = InColumn1;
		Columns[2] = InColumn2;
		Columns[3] = InColumn3;
	}

	Mat4x4 Mat4x4::operator*(const Mat4x4& InOther) const
	{
		Mat4x4 result;

		for (size_t Idx = 0; Idx < 4; Idx++)
			result.Columns[Idx] = Columns[0] * InOther.Columns[Idx][0] + Columns[1] * InOther.Columns[Idx][1] + Columns[2] * InOther.Columns[Idx][2] + Columns[3] * InOther.Columns[Idx][3];

		return result;
	}

	Mat4x4 Mat4x4::operator*(float InOther) const
	{
		Mat4x4 result;
		FVec4 scalar(InOther);

		for (size_t Idx = 0; Idx < 4; Idx++)
			result.Columns[Idx] = Columns[Idx] * scalar;

		return result;
	}

	Mat4x4 Mat4x4::CreateProjection(float InFieldOfView, float InWidth, float InHeight, float InNear, float InFar)
	{
		float rad = InFieldOfView;
		float h = cosf(0.5f * rad) / sinf(0.5f * rad);
		float w = h * InHeight / InWidth;

		Mat4x4 result(0);
		result[0][0] = w;
		result[1][1] = -h;
		result[2][2] = InFar / (InNear - InFar);
		result[2][3] = -1.0f;
		result[3][2] = -(InFar * InNear) / (InFar - InNear);
		return result;
	}

	Mat4x4 Mat4x4::CreateTranslation(const FVec3& InTranslation)
	{
		Mat4x4 result(1.0f);
		result[3][0] = InTranslation.X;
		result[3][1] = InTranslation.Y;
		result[3][2] = InTranslation.Z;
		return result;
	}

	Mat4x4 Mat4x4::FromQuat(const FQuat& InQuat)
	{
		Mat4x4 result(0.0f);

		float x = InQuat.Value.X;
		float y = InQuat.Value.Y;
		float z = InQuat.Value.Z;
		float w = InQuat.Value.W;

		float tx = x * 2.0f;
		float ty = y * 2.0f;
		float tz = z * 2.0f;

		float xx = tx * x;
		float yy = ty * y;
		float zz = tz * z;
		float xy = tx * y;
		float xz = tx * z;
		float xw = tx * w;
		float yz = ty * z;
		float yw = ty * w;
		float zw = tz * w;

		result[0] = FVec4((1.0f - yy) - zz, xy + zw, xz - yw, 0.0f);
		result[1] = FVec4(xy - zw, (1.0f - zz) - xx, yz + xw, 0.0f);
		result[2] = FVec4(xz + yw, yz - xw, (1.0f - xx) - yy, 0.0f);
		result[3][3] = 1.0f;

		return result;
	}

	Mat4x4 Mat4x4::Inversed() const
	{
		Mat4x4 result(*this);
		result.Inverse();
		return result;
	}

	void Mat4x4::Inverse()
	{
		float Coef00 = Columns[2][2] * Columns[3][3] - Columns[3][2] * Columns[2][3];
		float Coef02 = Columns[1][2] * Columns[3][3] - Columns[3][2] * Columns[1][3];
		float Coef03 = Columns[1][2] * Columns[2][3] - Columns[2][2] * Columns[1][3];

		float Coef04 = Columns[2][1] * Columns[3][3] - Columns[3][1] * Columns[2][3];
		float Coef06 = Columns[1][1] * Columns[3][3] - Columns[3][1] * Columns[1][3];
		float Coef07 = Columns[1][1] * Columns[2][3] - Columns[2][1] * Columns[1][3];

		float Coef08 = Columns[2][1] * Columns[3][2] - Columns[3][1] * Columns[2][2];
		float Coef10 = Columns[1][1] * Columns[3][2] - Columns[3][1] * Columns[1][2];
		float Coef11 = Columns[1][1] * Columns[2][2] - Columns[2][1] * Columns[1][2];

		float Coef12 = Columns[2][0] * Columns[3][3] - Columns[3][0] * Columns[2][3];
		float Coef14 = Columns[1][0] * Columns[3][3] - Columns[3][0] * Columns[1][3];
		float Coef15 = Columns[1][0] * Columns[2][3] - Columns[2][0] * Columns[1][3];

		float Coef16 = Columns[2][0] * Columns[3][2] - Columns[3][0] * Columns[2][2];
		float Coef18 = Columns[1][0] * Columns[3][2] - Columns[3][0] * Columns[1][2];
		float Coef19 = Columns[1][0] * Columns[2][2] - Columns[2][0] * Columns[1][2];

		float Coef20 = Columns[2][0] * Columns[3][1] - Columns[3][0] * Columns[2][1];
		float Coef22 = Columns[1][0] * Columns[3][1] - Columns[3][0] * Columns[1][1];
		float Coef23 = Columns[1][0] * Columns[2][1] - Columns[2][0] * Columns[1][1];

		FVec4 Fac0(Coef00, Coef00, Coef02, Coef03);
		FVec4 Fac1(Coef04, Coef04, Coef06, Coef07);
		FVec4 Fac2(Coef08, Coef08, Coef10, Coef11);
		FVec4 Fac3(Coef12, Coef12, Coef14, Coef15);
		FVec4 Fac4(Coef16, Coef16, Coef18, Coef19);
		FVec4 Fac5(Coef20, Coef20, Coef22, Coef23);

		FVec4 Vec0(Columns[1][0], Columns[0][0], Columns[0][0], Columns[0][0]);
		FVec4 Vec1(Columns[1][1], Columns[0][1], Columns[0][1], Columns[0][1]);
		FVec4 Vec2(Columns[1][2], Columns[0][2], Columns[0][2], Columns[0][2]);
		FVec4 Vec3(Columns[1][3], Columns[0][3], Columns[0][3], Columns[0][3]);

		FVec4 Inv0(Vec1 * Fac0 - Vec2 * Fac1 + Vec3 * Fac2);
		FVec4 Inv1(Vec0 * Fac0 - Vec2 * Fac3 + Vec3 * Fac4);
		FVec4 Inv2(Vec0 * Fac1 - Vec1 * Fac3 + Vec3 * Fac5);
		FVec4 Inv3(Vec0 * Fac2 - Vec1 * Fac4 + Vec2 * Fac5);

		FVec4 SignA(+1, -1, +1, -1);
		FVec4 SignB(-1, +1, -1, +1);
		Mat4x4 Inv(Inv0 * SignA, Inv1 * SignB, Inv2 * SignA, Inv3 * SignB);

		FVec4 Row0(Inv[0][0], Inv[1][0], Inv[2][0], Inv[3][0]);

		FVec4 Dot0(Columns[0] * Row0);
		float Dot1 = (Dot0.X + Dot0.Y) + (Dot0.Z + Dot0.W);

		if (IsNearZero(Dot1))
			return;

		float OneOverDeterminant = 1.0f / Dot1;

		Columns[0] = Inv.Columns[0] * OneOverDeterminant;
		Columns[1] = Inv.Columns[1] * OneOverDeterminant;
		Columns[2] = Inv.Columns[2] * OneOverDeterminant;
		Columns[3] = Inv.Columns[3] * OneOverDeterminant;
	}

	Mat4x4 Mat4x4::LookAt(const FVec3& InEye, const FVec3& InCenter, const FVec3& InUp)
	{
		FVec3 f = InCenter - InEye;
		f.Normalize();

		FVec3 s = f.Cross(InUp);
		s.Normalize();

		FVec3 u = s.Cross(f);

		Mat4x4 result(1.0f);
		result[0][0] = s.X;
		result[1][0] = s.Y;
		result[2][0] = s.Z;
		result[0][1] = u.X;
		result[1][1] = u.Y;
		result[2][1] = u.Z;
		result[0][2] = -f.X;
		result[1][2] = -f.Y;
		result[2][2] = -f.Z;
		result[3][0] = -s.Dot(InEye);
		result[3][1] = -u.Dot(InEye);
		result[3][2] =  f.Dot(InEye);

		return result;
	}

	
	//Mat4x4 Mat4x4::Transposed() const
	//{
	//}

}
