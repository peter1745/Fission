#include "FissionPCH.hpp"
#include "BroadPhase.hpp"
#include "Fission/Body/DynamicBody.hpp"

namespace Fission {

	int Compare(const void* InData0, const void* InData1)
	{
		const auto* body0 = reinterpret_cast<const BroadPhase::PsuedoBody*>(InData0);
		const auto* body1 = reinterpret_cast<const BroadPhase::PsuedoBody*>(InData1);
		return body0->Value < body1->Value ? -1 : 1;
	}

	void BroadPhase::Execute(const std::vector<std::unique_ptr<Body>>& InBodies, float InDeltaTime)
	{
		SortBodies(InBodies, InDeltaTime);
		BuildCollisionPairs();
	}

	void BroadPhase::SortBodies(const std::vector<std::unique_ptr<Body>>& InBodies, float InDeltaTime)
	{
		Math::FVec3 axis(1.0f);
		axis.Normalize();

		m_SortedArray.clear();

		if (InBodies.size() * 2 > m_SortedArray.size())
			m_SortedArray.resize(InBodies.size() * 2);

		for (size_t i = 0; i < InBodies.size(); i++)
		{
			const auto& body = InBodies[i];
			auto boundingBox = body->GetShape()->GetBoundingBox();

			Math::FVec3 linearVelocity(0.0f);

			if (body->GetType() == EBodyType::Dynamic)
				linearVelocity = body->As<DynamicBody>()->GetLinearVelocity();

			// Expand the bounds by the linear velocity
			boundingBox.Expand(boundingBox.MinBound + linearVelocity * InDeltaTime);
			boundingBox.Expand(boundingBox.MaxBound + linearVelocity * InDeltaTime);

			float epsilon = 0.01f;
			boundingBox.Expand(boundingBox.MinBound + Math::FVec3(-1.0f) * epsilon);
			boundingBox.Expand(boundingBox.MaxBound + Math::FVec3(1.0f) * epsilon);

			m_SortedArray[i * 2 + 0].ID = i;
			m_SortedArray[i * 2 + 0].Value = axis.Dot(boundingBox.MinBound);
			m_SortedArray[i * 2 + 0].IsMin = true;

			m_SortedArray[i * 2 + 1].ID = i;
			m_SortedArray[i * 2 + 1].Value = axis.Dot(boundingBox.MaxBound);
			m_SortedArray[i * 2 + 1].IsMin = false;
		}

		std::qsort(m_SortedArray.data(), InBodies.size() * 2, sizeof(PsuedoBody), Compare);
	}

	void BroadPhase::BuildCollisionPairs()
	{
		m_CollisionPairs.clear();

		for (size_t i = 0; i < m_SortedArray.size(); i++)
		{
			const auto& bodyA = m_SortedArray[i];

			if (!bodyA.IsMin)
				continue;

			CollisionPair pair;
			pair.A = bodyA.ID;

			for (size_t j = i + 1; j < m_SortedArray.size(); j++)
			{
				const auto& bodyB = m_SortedArray[j];

				if (bodyB.ID == bodyA.ID)
					break;

				if (!bodyB.IsMin)
					continue;

				pair.B = bodyB.ID;

				m_CollisionPairs.push_back(pair);
			}
		}
	}

}
