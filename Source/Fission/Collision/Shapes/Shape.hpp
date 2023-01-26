#pragma once

#include "Fission/Math/Vec3.hpp"

namespace Fission {

	enum class EShapeType { Sphere, Box };

	class Shape
	{
	public:
		virtual constexpr EShapeType GetType() const = 0;

		//const glm::mat3& GetInertiaTensor() const { return m_InertiaTensor; }
		const Math::Vec3& GetCenterOfMass() const { return m_CenterOfMass; } 

	protected:
		//glm::mat3 m_InertiaTensor;
		Math::Vec3 m_CenterOfMass = { 0.0f, 0.0f, 0.0f };
	};

}
