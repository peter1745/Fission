#pragma once

#include "Fission/Math/FVec3.hpp"
#include "Fission/Math/Mat3x3.hpp"
#include "Fission/Math/AABB.hpp"

namespace Fission {

	enum class EShapeType { Sphere };

	class Shape
	{
	public:
		virtual constexpr EShapeType GetType() const = 0;

		const Math::AABB& GetBoundingBox() const { return m_BoundingBox; }

		const Math::Mat3x3& GetInertiaTensor() const { return m_InertiaTensor; }
		const Math::FVec3& GetCenterOfMass() const { return m_CenterOfMass; } 

	protected:
		Math::Mat3x3 m_InertiaTensor;
		Math::FVec3 m_CenterOfMass = { 0.0f, 0.0f, 0.0f };
		Math::AABB m_BoundingBox;
	};

}
