#include "FissionPCH.hpp"
#include "ContactResolver.hpp"

#include "Fission/Body/DynamicBody.hpp"

#include "Shapes/SphereShape.hpp"
#include "Shapes/BoxShape.hpp"

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
			float InvMassA = 0.0f;
			float InvMassB = 0.0f;

			Math::Mat3x3 InvInertiaTensorA(0.0f);
			Math::Mat3x3 InvInertiaTensorB(0.0f);

			Math::FVec3 LinearVelocityA(0.0f);
			Math::FVec3 LinearVelocityB(0.0f);

			Math::FVec3 AngularVelocityA(0.0f);
			Math::FVec3 AngularVelocityB(0.0f);

			if (Contact.BodyA->GetType() == EBodyType::Dynamic)
			{
				auto* DynamicA = Contact.BodyA->As<DynamicBody>();
				InvMassA = DynamicA->GetInverseMass();
				InvInertiaTensorA = DynamicA->GetWorldSpaceInverseInertiaTensor();
				LinearVelocityA = DynamicA->GetLinearVelocity();
				AngularVelocityA = DynamicA->GetAngularVelocity();
				DynamicA->SetLinearVelocity(Math::FVec3::Zero());
			}

			if (Contact.BodyB->GetType() == EBodyType::Dynamic)
			{
				auto* DynamicB = Contact.BodyB->As<DynamicBody>();
				InvMassB = DynamicB->GetInverseMass();
				InvInertiaTensorB = DynamicB->GetWorldSpaceInverseInertiaTensor();
				LinearVelocityB = DynamicB->GetLinearVelocity();
				AngularVelocityB = DynamicB->GetAngularVelocity();
				DynamicB->SetLinearVelocity(Math::FVec3::Zero());
			}

			Math::FVec3 BodySpacePointA = Contact.WorldSpacePointA - Contact.BodyA->GetWorldSpaceCenterOfMass();
			Math::FVec3 BodySpacePointB = Contact.WorldSpacePointB - Contact.BodyB->GetWorldSpaceCenterOfMass();

			Math::FVec3 AngularImpulseA = (InvInertiaTensorA * BodySpacePointA.Cross(Contact.Normal)).Cross(BodySpacePointA);
			Math::FVec3 AngularImpulseB = (InvInertiaTensorB * BodySpacePointB.Cross(Contact.Normal)).Cross(BodySpacePointB);
			float AngularFactor = (AngularImpulseA + AngularImpulseB).Dot(Contact.Normal);

			Math::FVec3 VelocityA = LinearVelocityA + AngularVelocityA.Cross(BodySpacePointA);
			Math::FVec3 VelocityB = LinearVelocityB + AngularVelocityB.Cross(BodySpacePointB);

			Math::FVec3 RelativeVelocity = VelocityA - VelocityB;
			float Impulse = (1.0f + 0.0f) * RelativeVelocity.Dot(Contact.Normal) / (InvMassA + InvMassB + AngularFactor);
			Math::FVec3 ImpulseV = Contact.Normal * Impulse;

			if (Contact.BodyA->GetType() == EBodyType::Dynamic)
				Contact.BodyA->As<DynamicBody>()->AddImpulse(Contact.WorldSpacePointA, ImpulseV * -1.0f);

			if (Contact.BodyB->GetType() == EBodyType::Dynamic)
				Contact.BodyB->As<DynamicBody>()->AddImpulse(Contact.WorldSpacePointB, ImpulseV * 1.0f);

			float MassContributionA = InvMassA / (InvMassA + InvMassB);
			float MassContributionB = InvMassB / (InvMassA + InvMassB);

			Math::FVec3 Distance = Contact.WorldSpacePointB - Contact.WorldSpacePointA;
			Contact.BodyA->SetPosition(Contact.BodyA->GetPosition() + Distance * MassContributionA);
			Contact.BodyB->SetPosition(Contact.BodyB->GetPosition() - Distance * MassContributionB);
		}
	}

	Math::FVec3 ContactResolver::GetContactPosition(const Math::FVec3& InBodyPosition, const Math::FVec3& InNormal, const Shape* InShape) const
	{
		switch (InShape->GetType())
		{
		case EShapeType::Sphere: return InBodyPosition + InNormal * static_cast<const SphereShape*>(InShape)->GetRadius();
		case EShapeType::Box:
		{
			Math::AABB BoundingBox = static_cast<const BoxShape*>(InShape)->GetBoundingBox();

			int BestAxis = 0;
			float BestAxisWeight = 0.0f;

			for (int Idx = 0; Idx < 3; Idx++)
			{
				float Weight = abs(InNormal[Idx] / BoundingBox.MaxBound[Idx]);

				if (Weight > BestAxisWeight)
				{
					BestAxis = Idx;
					BestAxisWeight = Weight;
				}
			}

			return (InNormal * BoundingBox.MaxBound[BestAxis] / abs(InNormal[BestAxis])) + InBodyPosition;
		}
		}
	}

}
