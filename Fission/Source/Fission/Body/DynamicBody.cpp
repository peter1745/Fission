#include "FissionPCH.hpp"
#include "DynamicBody.hpp"

#include "Fission/Core/Assert.hpp"

namespace Fission {

	void DynamicBody::AddImpulse(const Math::FVec3& InPoint, const Math::FVec3& InImpulse)
	{
		AddLinearImpulse(InImpulse);

		auto position = GetWorldSpaceCenterOfMass();
		auto relativePoint = InPoint - position;
		AddAngularImpulse(relativePoint.Cross(InImpulse));
	}

	void DynamicBody::AddLinearImpulse(const Math::FVec3& InImpulse)
	{
		m_LinearVelocity += InImpulse * m_InvMass;
	}

	void DynamicBody::AddAngularImpulse(const Math::FVec3& InImpulse)
	{
		m_AngularVelocity += GetWorldSpaceInverseInertiaTensor() * InImpulse;

		constexpr float MaxAngularVelocity = 30.0f;
		if (m_AngularVelocity.LengthSq() > MaxAngularVelocity * MaxAngularVelocity)
		{
			m_AngularVelocity.Normalize();
			m_AngularVelocity *= MaxAngularVelocity;
		}
	}

	Math::Mat3x3 DynamicBody::GetWorldSpaceInverseInertiaTensor() const
	{
		Math::Mat3x3 rotation = Math::Mat3x3::FromQuat(m_Rotation);
		return rotation * m_InvInertia * rotation.Transposed();
	}

	void DynamicBody::UpdateInertiaTensor()
	{
		m_InvInertia = m_Shape->GetInertiaTensor().Inversed() * m_InvMass;
	}

}
