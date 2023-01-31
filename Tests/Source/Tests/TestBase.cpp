#include "TestBase.hpp"

#include <Fission/Collision/Shapes/SphereShape.hpp>

void TestBase::Start()
{
	m_IsRunning = true;

	Fission::PhysicsSettings settings;
	PopulateSettings(settings);

	m_PhysicsWorld = std::make_unique<Fission::PhysicsWorld>();
	m_PhysicsWorld->SetSettings(settings);
	m_PhysicsWorld->Init();

	OnStart();

	// Create Actor Data
	for (const auto& body : m_PhysicsWorld->GetBodies())
	{
		auto& actor = m_Actors.emplace_back();
		actor.PhysicsBody = body.get();

		const auto* shape = body->GetShape();

		if (shape == nullptr)
			continue;

		switch (shape->GetType())
		{
		case Fission::EShapeType::Sphere:
		{
			const auto* sphereShape = static_cast<const Fission::SphereShape*>(shape);
			actor.Mesh = MeshGenerator::GenerateCubeSphere(sphereShape->GetRadius(), 8, false, true);
			break;
		}
		}
	}
}

void TestBase::Stop()
{
	OnStop();

	m_Actors.clear();

	m_PhysicsWorld->Shutdown();
	m_PhysicsWorld.reset();

	m_IsRunning = false;
}

void TestBase::Simulate(float InDeltaTime)
{
	OnPreSimulate(InDeltaTime);
	m_PhysicsWorld->Simulate(InDeltaTime);
	OnPostSimulate(InDeltaTime);
}

float TestBase::RandomFloat(float InLow, float InHigh) const
{
	return InLow + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (InHigh - InLow)));
}
