#pragma once

#include "Renderer/MeshGenerator.hpp"

#include <Fission/World/PhysicsWorld.hpp>

struct Actor
{
	MeshData Mesh;
	Fission::Body* PhysicsBody;
};

class TestBase
{
public:
	virtual void Load() {}
	virtual void Unload() {}

	void Start();
	void Stop();

	void Simulate(float InDeltaTime);

	bool IsRunning() const { return m_IsRunning; }
	bool NeedsRestart() const { return m_NeedsRestart; }
	void SetNeedsRestart(bool value) { m_NeedsRestart = value; }

	Fission::PhysicsWorld* GetPhysicsWorld() const { return m_PhysicsWorld.get(); }

	const auto& GetActors() const { return m_Actors; }

	virtual void DrawUI() {}

	float RandomFloat(float InLow, float InHigh) const;

private:
	virtual void PopulateSettings(Fission::PhysicsSettings& InSettings) {}
	
	virtual void OnStart() {}
	virtual void OnStop() {}

	virtual void OnPreSimulate(float InDeltaTime) {}
	virtual void OnPostSimulate(float InDeltaTime) {}


protected:
	std::unique_ptr<Fission::PhysicsWorld> m_PhysicsWorld = nullptr;
	std::vector<Actor> m_Actors;

	bool m_NeedsRestart = false;

private:
	bool m_IsRunning = false;
};
