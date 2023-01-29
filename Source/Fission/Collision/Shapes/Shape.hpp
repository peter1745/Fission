#pragma once

#include "Fission/Math/FVec3.hpp"
#include "Fission/Math/Mat3x3.hpp"

namespace Fission {

	enum class EShapeType { Sphere, Box };

	class Shape
	{
	public:
		virtual constexpr EShapeType GetType() const = 0;

		const Math::Mat3x3& GetInertiaTensor() const { return m_InertiaTensor; }
		const Math::FVec3& GetCenterOfMass() const { return m_CenterOfMass; } 

	protected:
		Math::Mat3x3 m_InertiaTensor;
		Math::FVec3 m_CenterOfMass = { 0.0f, 0.0f, 0.0f };
	};

}
