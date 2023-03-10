#pragma once

#include "Fission/Body/Body.hpp"

#include <vector>

namespace Fission {

	struct ContactPair
	{
		Math::FVec3 WorldSpacePointA; // Hit point on body A in world space
		Math::FVec3 WorldSpacePointB; // Hit point on body B in world space
		
		Math::FVec3 Normal; // Contact normal in world space
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
		Math::FVec3 GetContactPosition(const Math::FVec3& InBodyPosition, const Math::FVec3& InNormal, const Shape* InShape) const;

	private:
		std::vector<ContactPair> m_Contacts;
	};

}
