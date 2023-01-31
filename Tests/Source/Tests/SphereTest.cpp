#include "SphereTest.hpp"

#include <Fission/Collision/Shapes/SphereShape.hpp>

void SphereTest::OnStart()
{
	Fission::BodySettings settings;
	settings.BodyType = Fission::EBodyType::Dynamic;
	settings.Position = { 0.0f, 0.0f, -10.0f };
	settings.CollisionShape = new Fission::SphereShape(0.5f);
	m_PhysicsWorld->CreateBody(settings);

	Fission::BodySettings groundSettings;
	groundSettings.BodyType = Fission::EBodyType::Static;
	groundSettings.Position = { 0.0f, -10.0f, -10.0f };
	groundSettings.CollisionShape = new Fission::SphereShape(5.0f);
	m_PhysicsWorld->CreateBody(groundSettings);
}

void SphereTest::OnStop()
{
}
