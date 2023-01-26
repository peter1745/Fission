#pragma once

#include "Body.hpp"

namespace Fission {

	class DynamicBody : public Body
	{
	public:
		constexpr EBodyType GetType() const override { return EBodyType::Dynamic; }

		const Math::Vec3& GetLinearVelocity() const { return m_LinearVelocity; }
		void SetLinearVelocity(const Math::Vec3& InLinearVelocity) { m_LinearVelocity = InLinearVelocity; }

		void AddAngularImpulse(const Math::Vec3& InImpulse);

		float GetInverseMass() const { return m_InvMass; }
		float GetMass() const { return 1.0f / m_InvMass; }
		void SetMass(float InMass)
		{
			m_InvMass = 1.0f / InMass;
			UpdateInertiaTensor();
		}

		const Math::Vec3& GetAccumulatedForce() const { return m_Force; }

		//const glm::mat3& GetInverseInertiaTensor() const { return m_InvInertia; }
		//glm::mat3 GetWorldSpaceInverseInertiaTensor() const;

		const Math::Vec3& GetCenterOfMass() const { return m_Shape->GetCenterOfMass(); }
		//Vec3 GetWorldSpaceCenterOfMass() const { return m_Position + (m_Rotation * m_Shape->GetCenterOfMass()); }

	private:
		void UpdateInertiaTensor();

	private:
		float m_InvMass;
		Math::Vec3 m_LinearVelocity = { 0.0f, 0.0f, 0.0f };
		Math::Vec3 m_AngularVelocity = { 0.0f, 0.0f, 0.0f };
		Math::Vec3 m_Force = { 0.0f, 0.0f, 0.0f };

		//glm::mat3 m_InvInertia;

	private:
		friend class PhysicsWorld;
	};

}
