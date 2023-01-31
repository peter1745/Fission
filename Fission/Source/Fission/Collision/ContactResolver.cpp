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

		Contact.Normal = (InBodyB->GetPosition() - InBodyA->GetPosition()).Normalized();

		Contact.WorldSpacePointA = GetContactPosition(InBodyA->GetPosition(), Contact.Normal, InBodyA->GetShape());
		Contact.WorldSpacePointB = GetContactPosition(InBodyB->GetPosition(), -Contact.Normal, InBodyB->GetShape());
	}

	void ContactResolver::ResolveContacts()
	{
		for (auto& Contact : m_Contacts)
		{
			float invMassA = 0.0f;
			float invMassB = 0.0f;

			Math::FVec3 linearVelocityA(0.0f);
			Math::FVec3 linearVelocityB(0.0f);

			if (Contact.BodyA->GetType() == EBodyType::Dynamic)
			{
				auto* dynamicA = Contact.BodyA->As<DynamicBody>();
				invMassA = dynamicA->GetInverseMass();
				linearVelocityA = dynamicA->GetLinearVelocity();
			}

			if (Contact.BodyB->GetType() == EBodyType::Dynamic)
			{
				auto* dynamicB = Contact.BodyB->As<DynamicBody>();
				invMassB = dynamicB->GetInverseMass();
				linearVelocityB = dynamicB->GetLinearVelocity();
			}

			float combinedRestitution = Contact.BodyA->GetResititution() * Contact.BodyB->GetResititution();

			auto velocityDiff = linearVelocityA - linearVelocityB;
			float impulseJ = -(1.0f + combinedRestitution) * velocityDiff.Dot(Contact.Normal) / (invMassA + invMassB);
			auto vectorImpulseJ = Contact.Normal * impulseJ;

			if (Contact.BodyA->GetType() == EBodyType::Dynamic)
				Contact.BodyA->As<DynamicBody>()->AddLinearImpulse(vectorImpulseJ);

			if (Contact.BodyB->GetType() == EBodyType::Dynamic)
				Contact.BodyB->As<DynamicBody>()->AddLinearImpulse(-vectorImpulseJ);

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
