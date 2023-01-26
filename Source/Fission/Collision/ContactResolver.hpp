#pragma once

#include "Fission/Body/Body.hpp"

#include <vector>

namespace Fission {

	struct ContactPair
	{
		Math::Vec3 WorldSpacePointA; // Hit point on body A in world space
		Math::Vec3 WorldSpacePointB; // Hit point on body B in world space
		
		Math::Vec3 Normal; // Contact normal in world space
		float SeparationDistance; // Positive when non-penetrating, Negative when penetrating

		float ImpactTime;

		Body* BodyA;
		Body* BodyB;
	};

	class ContactResolver
	{
	public:
		void GenerateContact(Body* InBodyA, Body* InBodyB);

		void ResolveContacts();

	private:
		Math::Vec3 GetContactPosition(const Math::Vec3& InBodyPosition, const Math::Vec3& InNormal, const Shape* InShape) const;

	private:
		std::vector<ContactPair> m_Contacts;
	};

}
