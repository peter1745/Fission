#include "SpheresStressTest.hpp"

#include <Fission/Collision/Shapes/SphereShape.hpp>

#include <iostream>

#include <imgui.h>

static int32_t s_ColumnHeight = 50;

void SpheresStressTest::DrawUI()
{
	m_NeedsRestart |= ImGui::DragInt("Column Height (in Spheres)", &s_ColumnHeight);
}

void SpheresStressTest::OnStart()
{
	for (int32_t x = -5; x < 5; x++)
	{
		for (int32_t z = -5; z < 5; z++)
		{
			Fission::BodySettings groundSettings;
			groundSettings.BodyType = Fission::EBodyType::Static;
			groundSettings.Position = { x * 80.0f, -40.0f, z * 80.0f };
			groundSettings.CollisionShape = new Fission::SphereShape(60.0f);
			m_PhysicsWorld->CreateBody(groundSettings);
		}
	}

	uint32_t sphereCount = 0;

	for (int32_t xCol = -1; xCol < 1; xCol++)
	{
		for (int32_t zCol = -1; zCol < 1; zCol++)
		{
			for (int32_t y = 0; y < s_ColumnHeight; y++)
			{
				Fission::BodySettings settings;
				settings.BodyType = Fission::EBodyType::Dynamic;
				settings.Position = { xCol * 20.0f, y * 2.5f + 35.0f, zCol * 20.0f };
				settings.CollisionShape = new Fission::SphereShape(1.0f);
				m_PhysicsWorld->CreateBody(settings);

				sphereCount++;
			}
		}
	}

	std::cout << "Generated " << sphereCount << " spheres\n";
}

void SpheresStressTest::OnStop()
{
}
