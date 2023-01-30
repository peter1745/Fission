#include "FissionPCH.hpp"
#include "ContactResolver.hpp"

#include "Fission/Body/DynamicBody.hpp"

#include "Shapes/SphereShape.hpp"

#include <array>

#include <iostream>
#include <format>

namespace Fission {

	void ContactResolver::GenerateContact(Body* InBodyA, Body* InBodyB)
	{
		auto& Contact = m_Contacts.emplace_back();
		Contact.BodyA = InBodyA;
		Contact.BodyB = InBodyB;

		Contact.Normal = InBodyB->GetPosition() - InBodyA->GetPosition();

		Contact.WorldSpacePointA = GetContactPosition(InBodyA->GetPosition(), Contact.Normal, InBodyA->GetShape());
		Contact.WorldSpacePointB = GetContactPosition(InBodyB->GetPosition(), -Contact.Normal, InBodyB->GetShape());
	}

	void ContactResolver::ResolveContacts()
	{
		for (auto& Contact : m_Contacts)
		{
			float invMassA = 0.0f;
			float invMassB = 0.0f;

			if (Contact.BodyA->GetType() == EBodyType::Dynamic)
			{
				auto* dynamicA = Contact.BodyA->As<DynamicBody>();
				invMassA = dynamicA->GetInverseMass();
				dynamicA->SetLinearVelocity(Math::FVec3::Zero());
			}

			if (Contact.BodyB->GetType() == EBodyType::Dynamic)
			{
				auto* dynamicB = Contact.BodyB->As<DynamicBody>();
				invMassB = dynamicB->GetInverseMass();
				dynamicB->SetLinearVelocity(Math::FVec3::Zero());
			}

			float massContributionA = invMassA / (invMassA + invMassB);
			float massContributionB = invMassB / (invMassA + invMassB);

			Math::FVec3 distance = Contact.WorldSpacePointB - Contact.WorldSpacePointA;
			Contact.BodyA->SetPosition(Contact.BodyA->GetPosition() + distance * massContributionA);
			Contact.BodyB->SetPosition(Contact.BodyB->GetPosition() - distance * massContributionB);
		}
	}

	Math::FVec3 ContactResolver::GetContactPosition(const Math::FVec3& InBodyPosition, const Math::FVec3& InNormal, const Shape* InShape) const
	{
		switch (InShape->GetType())
		{
		case EShapeType::Sphere: return InBodyPosition + InNormal * static_cast<const SphereShape*>(InShape)->GetRadius();
		}
	}

}
