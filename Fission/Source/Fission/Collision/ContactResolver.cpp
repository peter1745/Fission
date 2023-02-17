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

			Math::FVec3 angularVelocityA(0.0f);
			Math::FVec3 angularVelocityB(0.0f);

			Math::Mat3x3 invWorldInertiaA(0.0f);
			Math::Mat3x3 invWorldInertiaB(0.0f);

			if (Contact.BodyA->GetType() == EBodyType::Dynamic)
			{
				auto* dynamicA = Contact.BodyA->As<DynamicBody>();
				invMassA = dynamicA->GetInverseMass();
				linearVelocityA = dynamicA->GetLinearVelocity();
				angularVelocityA = dynamicA->GetAngularVelocity();
				invWorldInertiaA = dynamicA->GetWorldSpaceInverseInertiaTensor();
			}

			if (Contact.BodyB->GetType() == EBodyType::Dynamic)
			{
				auto* dynamicB = Contact.BodyB->As<DynamicBody>();
				invMassB = dynamicB->GetInverseMass();
				linearVelocityB = dynamicB->GetLinearVelocity();
				angularVelocityB = dynamicB->GetAngularVelocity();
				invWorldInertiaB = dynamicB->GetWorldSpaceInverseInertiaTensor();
			}

			float combinedRestitution = Contact.BodyA->GetResititution() * Contact.BodyB->GetResititution();

			Math::FVec3 ra = Contact.WorldSpacePointA - Contact.BodyA->GetWorldSpaceCenterOfMass();
			Math::FVec3 rb = Contact.WorldSpacePointB - Contact.BodyB->GetWorldSpaceCenterOfMass();

			Math::FVec3 angularJA = (invWorldInertiaA * ra.Cross(Contact.Normal)).Cross(ra);
			Math::FVec3 angularJB = (invWorldInertiaB * rb.Cross(Contact.Normal)).Cross(rb);
			float angularFactor = (angularJA + angularJB).Dot(Contact.Normal);

			Math::FVec3 velocityA = linearVelocityA + angularVelocityA.Cross(ra);
			Math::FVec3 velocityB = linearVelocityB + angularVelocityB.Cross(rb);

			float combinedFriction = Contact.BodyA->GetFriction() * Contact.BodyB->GetFriction();
			auto velocityDiff = velocityA - velocityB;

			float impulseJ = -(1.0f + combinedRestitution) * velocityDiff.Dot(Contact.Normal) / (invMassA + invMassB);
			auto vectorImpulseJ = Contact.Normal * impulseJ;

			if (Contact.BodyA->GetType() == EBodyType::Dynamic)
				Contact.BodyA->As<DynamicBody>()->AddImpulse(Contact.WorldSpacePointA, vectorImpulseJ);

			if (Contact.BodyB->GetType() == EBodyType::Dynamic)
				Contact.BodyB->As<DynamicBody>()->AddImpulse(Contact.WorldSpacePointB, -vectorImpulseJ);

			Math::FVec3 velocityNormal = Contact.Normal * Contact.Normal.Dot(velocityDiff);
			Math::FVec3 velocityTangent = velocityDiff - velocityNormal;
			Math::FVec3 relativeTangent = velocityTangent.Normalized();

			Math::FVec3 inertiaA = (invWorldInertiaA * ra.Cross(relativeTangent)).Cross(ra);
			Math::FVec3 inertiaB = (invWorldInertiaB * rb.Cross(relativeTangent)).Cross(rb);
			float inverseInertia = (inertiaA + inertiaB).Dot(relativeTangent);

			float reducedMass = 1.0f / (invMassA + invMassB + inverseInertia);
			Math::FVec3 impulseFriction = velocityTangent * reducedMass * combinedFriction;

			if (Contact.BodyA->GetType() == EBodyType::Dynamic)
				Contact.BodyA->As<DynamicBody>()->AddImpulse(Contact.WorldSpacePointA, -impulseFriction);

			if (Contact.BodyB->GetType() == EBodyType::Dynamic)
				Contact.BodyB->As<DynamicBody>()->AddImpulse(Contact.WorldSpacePointB, impulseFriction);

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
