#pragma once

#include "Shapes/SphereShape.hpp"

namespace Fission {

	class CollisionChecker
	{
	public:
		static bool TestSphereSphereCollision(const Math::FVec3& InSphereCenter0, const SphereShape* InSphere0, const Math::FVec3& InSphereCenter1, const SphereShape* InSphere1);
		//static bool TestSphereBoxCollision(const Math::FVec3& InSphereCenter, const SphereShape* InSphere, const Math::FVec3& InBoxCenter, const BoxShape* InBox);
		//static bool TestBoxBoxCollision(const Math::FVec3& InBoxCenter0, const BoxShape* InBox0, const Math::FVec3& InBoxCenter1, const BoxShape* InBox1);
	};

}
