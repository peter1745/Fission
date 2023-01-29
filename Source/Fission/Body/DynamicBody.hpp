#pragma once

#include "Body.hpp"

#include "Fission/Math/Mat3x3.hpp"

namespace Fission {

	class DynamicBody : public Body
	{
	public:
		constexpr EBodyType GetType() const override { return EBodyType::Dynamic; }

		const Math::FVec3& GetLinearVelocity() const { return m_LinearVelocity; }
		void SetLinearVelocity(const Math::FVec3& InLinearVelocity) { m_LinearVelocity = InLinearVelocity; }

		const Math::FVec3& GetAngularVelocity() const { return m_AngularVelocity; }
		void SetAngularVelocity(const Math::FVec3& InAngularVelocity) { m_AngularVelocity = InAngularVelocity; }

		void AddImpulse(const Math::FVec3& InPoint, const Math::FVec3& InImpulse);
		void AddLinearImpulse(const Math::FVec3& InImpulse);
		void AddAngularImpulse(const Math::FVec3& InImpulse);

		float GetInverseMass() const { return m_InvMass; }
		float GetMass() const { return 1.0f / m_InvMass; }
		void SetMass(float InMass)
		{
			m_InvMass = 1.0f / InMass;
			UpdateInertiaTensor();
		}

		const Math::FVec3& GetAccumulatedForce() const { return m_Force; }

		const Math::Mat3x3& GetInverseInertiaTensor() const { return m_InvInertia; }
		Math::Mat3x3 GetWorldSpaceInverseInertiaTensor() const;

	private:
		void UpdateInertiaTensor();

	private:
		float m_InvMass;
		Math::FVec3 m_LinearVelocity = { 0.0f, 0.0f, 0.0f };
		Math::FVec3 m_AngularVelocity = { 0.0f, 0.0f, 0.0f };
		Math::FVec3 m_Force = { 0.0f, 0.0f, 0.0f };

		Math::Mat3x3 m_InvInertia;

	private:
		friend class PhysicsWorld;
	};

}
