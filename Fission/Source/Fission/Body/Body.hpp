#pragma once

#include "Fission/Collision/Shapes/Shape.hpp"
#include "Fission/Math/FVec3.hpp"
#include "Fission/Math/FQuat.hpp"

#include <memory>

namespace Fission {

	enum class EBodyType : uint8_t { Static, Dynamic, Kinematic };

	struct BodySettings
	{
		EBodyType BodyType = EBodyType::Dynamic;
		Math::FVec3 Position = { 0.0f, 0.0f, 0.0f };
		Math::FQuat Rotation = { 0.0f, 0.0f, 0.0f, 1.0f };
		float Mass = 1000.0f;
		Shape* CollisionShape = nullptr;

		float Friction = 0.2f;
		float Restitution = 0.1f;
	};

	class Body
	{
	public:
		const Math::FVec3& GetPosition() const { return m_Position; }
		void SetPosition(const Math::FVec3& InPosition) { m_Position = InPosition; }

		const Math::FQuat& GetRotation() const { return m_Rotation; }
		void SetRotation(const Math::FQuat& InRotation) { m_Rotation = InRotation; }

		virtual constexpr EBodyType GetType() const { return EBodyType::Static; }

		template<typename TBodyType>
		TBodyType* As()
		{
			return static_cast<TBodyType*>(this);
		}

		const Math::FVec3& GetCenterOfMass() const { return m_Shape->GetCenterOfMass(); }
		Math::FVec3 GetWorldSpaceCenterOfMass() const { return m_Position + (m_Rotation * m_Shape->GetCenterOfMass()); }

		float GetFriction() const { return m_Restitution; }
		float GetResititution() const { return m_Restitution; }

	public:
		Shape* GetShape() const { return m_Shape; }

	protected:
		Math::FVec3 m_Position;
		Math::FQuat m_Rotation;

		// TODO(Peter): Make this a unique_ptr, body should own it's shape
		Shape* m_Shape = nullptr;

		float m_Friction;
		float m_Restitution;

	private:
		friend class PhysicsWorld;
	};

}
