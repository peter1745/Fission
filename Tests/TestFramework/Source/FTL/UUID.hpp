#pragma once

#include <functional>
#include <random>

namespace FTL {

	inline static std::random_device s_RandomDevice;
	inline static std::mt19937_64 s_RandomEngine(s_RandomDevice());
	inline static std::uniform_int_distribution<uint64_t> s_UniformDistribution;

	class UUID
	{
	public:
		static const uint64_t Invalid = 0;

	public:
		UUID()
		    : m_ID(s_UniformDistribution(s_RandomEngine)) {}

		UUID(uint64_t InID)
		    : m_ID(InID) {}

		UUID(const UUID& InOther)
		    : m_ID(InOther.m_ID) {}

		UUID(UUID&& InOther) noexcept
		    : m_ID(std::move(InOther.m_ID)) {}

		UUID& operator=(const UUID& InOther)
		{
			m_ID = InOther.m_ID;
			return *this;
		}

		UUID& operator=(UUID&& InOther) noexcept
		{
			m_ID = std::move(InOther.m_ID);
			return *this;
		}

		operator uint64_t() { return m_ID; }
		operator const uint64_t() const { return m_ID; }

	private:
		uint64_t m_ID;
	};

}

namespace std {

	template<>
	struct hash<FTL::UUID>
	{
		size_t operator()(const FTL::UUID& InUUID) const noexcept
		{
			return static_cast<size_t>(static_cast<uint64_t>(InUUID));
		}
	};

}
