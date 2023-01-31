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

		if (InBodies.size() * 2 > m_SortedArray.capacity())
			m_SortedArray.reserve(InBodies.size() * 2);

		for (size_t i = 0; i < InBodies.size(); i++)
		{
			const auto& body = InBodies[i];
			auto boundingBox = body->GetShape()->GetBoundingBox();
			boundingBox.Offset(body->GetPosition());

			Math::FVec3 linearVelocity(0.0f);

			if (body->GetType() == EBodyType::Dynamic)
				linearVelocity = body->As<DynamicBody>()->GetLinearVelocity();

			// Expand the bounds by the linear velocity
			boundingBox.Expand(boundingBox.MinBound + linearVelocity * InDeltaTime);
			boundingBox.Expand(boundingBox.MaxBound + linearVelocity * InDeltaTime);

			float epsilon = 0.01f;
			boundingBox.Expand(boundingBox.MinBound + Math::FVec3(-1.0f) * epsilon);
			boundingBox.Expand(boundingBox.MaxBound + Math::FVec3(1.0f) * epsilon);

			auto& psuedoBody0 = m_SortedArray.emplace_back();
			psuedoBody0.ID = i;
			psuedoBody0.Value = axis.Dot(boundingBox.MinBound);
			psuedoBody0.IsMin = true;
			psuedoBody0.IsStatic = body->GetType() == EBodyType::Static;

			auto& psuedoBody1 = m_SortedArray.emplace_back();
			psuedoBody1.ID = i;
			psuedoBody1.Value = axis.Dot(boundingBox.MaxBound);
			psuedoBody1.IsMin = false;
			psuedoBody1.IsStatic = body->GetType() == EBodyType::Static;
		}

		std::qsort(m_SortedArray.data(), m_SortedArray.size(), sizeof(PsuedoBody), Compare);
	}

	void BroadPhase::BuildCollisionPairs()
	{
		m_CollisionPairs.clear();

		if (m_SortedArray.size() > m_CollisionPairs.capacity())
			m_CollisionPairs.reserve(m_SortedArray.size());

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

				if (bodyA.IsStatic && bodyB.IsStatic)
					continue;

				pair.B = bodyB.ID;

				m_CollisionPairs.emplace_back(pair);
			}
		}
	}

}
