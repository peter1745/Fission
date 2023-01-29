#include "FissionPCH.hpp"
#include "DynamicBody.hpp"

namespace Fission {

	void DynamicBody::AddImpulse(const Math::FVec3& InPoint, const Math::FVec3& InImpulse)
	{
		AddLinearImpulse(InImpulse);

		auto Position = GetWorldSpaceCenterOfMass();
		auto RelativePoint = InPoint - Position;
		AddAngularImpulse(RelativePoint.Cross(InImpulse));
	}

	void DynamicBody::AddLinearImpulse(const Math::FVec3& InImpulse)
	{
		m_LinearVelocity += InImpulse * m_InvMass;
	}

	void DynamicBody::AddAngularImpulse(const Math::FVec3& InImpulse)
	{
		m_AngularVelocity += GetWorldSpaceInverseInertiaTensor() * InImpulse;
	}

	Math::Mat3x3 DynamicBody::GetWorldSpaceInverseInertiaTensor() const
	{
		Math::Mat3x3 Rotation = Math::Mat3x3::FromQuat(m_Rotation);
		return Rotation * m_InvInertia * Rotation.Transposed();
	}

	void DynamicBody::UpdateInertiaTensor()
	{
		m_InvInertia = m_Shape->GetInertiaTensor().Inversed() * m_InvMass;
	}

}
