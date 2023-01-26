#include "FissionPCH.hpp"
#include "CollisionChecker.hpp"

#include <iostream>
#include <format>

namespace Fission {

	bool CollisionChecker::TestSphereSphereCollision(const Math::Vec3& InSphereCenter0, const SphereShape* InSphere0, const Math::Vec3& InSphereCenter1, const SphereShape* InSphere1)
	{
		// We use dot(Distance, Distance) < RadiusSum^2 so we don't have to do a sqrt (may or may not make a big difference)
		float RadiusSum = InSphere0->GetRadius() + InSphere1->GetRadius();
		Math::Vec3 Distance = InSphereCenter0 - InSphereCenter1;

		float D = Distance.LengthSq();
		std::cout << std::format("RadiusSum^2={}, D={}", RadiusSum * RadiusSum, D) << std::endl;
		return D <= RadiusSum * RadiusSum;
	}

	bool CollisionChecker::TestSphereBoxCollision(const Math::Vec3& InSphereCenter, const SphereShape* InSphere, const Math::Vec3& InBoxCenter, const BoxShape* InBox)
	{
		Math::AABB BoundingBox = InBox->GetBoundingBox();
		BoundingBox.Offset(InBoxCenter);

		Math::Vec3 ClosestPoint = Math::Vec3::Max(BoundingBox.MinBound, Math::Vec3::Min(InSphereCenter, BoundingBox.MaxBound));
		Math::Vec3 Distance = ClosestPoint - InSphereCenter;

		return Distance.LengthSq() < InSphere->GetRadius() * InSphere->GetRadius();
	}

	bool CollisionChecker::TestBoxBoxCollision(const Math::Vec3& InBoxCenter0, const BoxShape* InBox0, const Math::Vec3& InBoxCenter1, const BoxShape* InBox1)
	{
		Math::AABB BoundingBox0 = InBox0->GetBoundingBox();
		Math::AABB BoundingBox1 = InBox1->GetBoundingBox();

		BoundingBox0.Offset(InBoxCenter0);
		BoundingBox1.Offset(InBoxCenter1);

		return BoundingBox0.IsAABBInside(BoundingBox1);
	}

}
