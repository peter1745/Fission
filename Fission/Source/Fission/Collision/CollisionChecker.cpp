#include "FissionPCH.hpp"
#include "CollisionChecker.hpp"

#include <iostream>
#include <format>

namespace Fission {

	bool CollisionChecker::TestSphereSphereCollision(const Math::FVec3& InSphereCenter0, const SphereShape* InSphere0, const Math::FVec3& InSphereCenter1, const SphereShape* InSphere1)
	{
		// We use dot(Distance, Distance) < RadiusSum^2 so we don't have to do a sqrt (may or may not make a big difference)
		float radiusSum = InSphere0->GetRadius() + InSphere1->GetRadius();
		Math::FVec3 distance = InSphereCenter0 - InSphereCenter1;
		return distance.LengthSq() <= radiusSum * radiusSum;
	}

	/*bool CollisionChecker::TestSphereBoxCollision(const Math::FVec3& InSphereCenter, const SphereShape* InSphere, const Math::FVec3& InBoxCenter, const BoxShape* InBox)
	{
		Math::AABB boundingBox = InBox->GetBoundingBox();
		boundingBox.Offset(InBoxCenter);

		Math::FVec3 closestPoint = Math::FVec3::Max(boundingBox.MinBound, Math::FVec3::Min(InSphereCenter, boundingBox.MaxBound));
		Math::FVec3 distance = closestPoint - InSphereCenter;

		return distance.LengthSq() < InSphere->GetRadius() * InSphere->GetRadius();
	}

	bool CollisionChecker::TestBoxBoxCollision(const Math::FVec3& InBoxCenter0, const BoxShape* InBox0, const Math::FVec3& InBoxCenter1, const BoxShape* InBox1)
	{
		Math::AABB boundingBox0 = InBox0->GetBoundingBox();
		Math::AABB boundingBox1 = InBox1->GetBoundingBox();

		boundingBox0.Offset(InBoxCenter0);
		boundingBox1.Offset(InBoxCenter1);

		return boundingBox0.IsAABBInside(boundingBox1);
	}*/

}
