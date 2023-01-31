#pragma once

#include "Fission/Body/Body.hpp"

#include <vector>

namespace Fission {

	class BroadPhase
	{
	public:
		struct PsuedoBody
		{
			int32_t ID;
			float Value;
			bool IsMin;
			bool IsStatic;
		};

		struct CollisionPair
		{
			int32_t A;
			int32_t B;

			bool operator==(const CollisionPair& InOther) const
			{
				return ((A == InOther.A) && (B == InOther.B)) || ((A == InOther.B) && (B == InOther.A));
			}

			bool operator!=(const CollisionPair& InOther) const
			{
				return !(*this == InOther);
			}
		};

	public:
		void Execute(const std::vector<std::unique_ptr<Body>>& InBodies, float InDeltaTime);

		const std::vector<CollisionPair>& GetCollisionPairs() const { return m_CollisionPairs; }

	private:
		void SortBodies(const std::vector<std::unique_ptr<Body>>& InBodies, float InDeltaTime);
		void BuildCollisionPairs();

	private:
		std::vector<PsuedoBody> m_SortedArray;
		std::vector<CollisionPair> m_CollisionPairs;
	};

}
