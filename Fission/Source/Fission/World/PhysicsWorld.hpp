#pragma once

#include "Fission/Body/Body.hpp"
#include "Fission/Integration/Integrator.hpp"
#include "Fission/Collision/BroadPhase/BroadPhase.hpp"

#include <vector>
#include <memory>

namespace Fission {

	struct PhysicsSettings
	{
		Math::FVec3 Gravity = { 0.0f, -9.81f, 0.0f };
	};

	struct PhysicsStats
	{
		uint32_t NumCollisionPairs = 0;
		uint32_t NumContacts = 0;
	};

	class PhysicsWorld
	{
	public:
		PhysicsWorld() = default;

		void Init();
		void Shutdown();

	public:
		void Simulate(float InDeltaTime);

		Body* CreateBody(const BodySettings& InSettings);

		const auto& GetBodies() const { return m_Bodies; }

	public:
		PhysicsSettings& GetSettings() { return m_Settings; }
		const PhysicsSettings& GetSettings() const { return m_Settings; }
		void SetSettings(const PhysicsSettings& InSettings) { m_Settings = InSettings; }

		const PhysicsStats& GetStats() const { return m_Stats; }

	private:
		PhysicsSettings m_Settings;
		PhysicsStats m_Stats;

		std::unique_ptr<Integrator> m_BodyIntegrator = nullptr;
		std::unique_ptr<BroadPhase> m_BroadPhase = nullptr;

		std::vector<std::unique_ptr<Body>> m_Bodies;
		float m_DeltaTime = 0.0f;
	};

}
