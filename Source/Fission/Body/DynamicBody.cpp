#include "FissionPCH.hpp"
#include "DynamicBody.hpp"

namespace Fission {

	void DynamicBody::AddAngularImpulse(const Math::Vec3& InImpulse)
	{
		//m_AngularVelocity += GetWorldSpaceInverseInertiaTensor() * InImpulse;
	}

	//glm::mat3 DynamicBody::GetWorldSpaceInverseInertiaTensor() const
	//{
	//	/*glm::mat3 Rotation = glm::toMat3(m_Rotation);
	//	return Rotation * m_InvInertia * glm::transpose(Rotation);*/
	//	return glm::mat3(0.0f);
	//}

	void DynamicBody::UpdateInertiaTensor()
	{
		//m_InvInertia = glm::inverse(m_Shape->GetInertiaTensor()) * m_InvMass;
	}

}
