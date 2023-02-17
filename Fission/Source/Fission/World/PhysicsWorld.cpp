#include "FissionPCH.hpp"
#include "PhysicsWorld.hpp"

#include "Fission/Body/DynamicBody.hpp"
#include "Fission/Integration/SemiImplicitEulerIntegrator.hpp"
#include "Fission/Collision/CollisionChecker.hpp"
#include "Fission/Collision/ContactResolver.hpp"

namespace Fission {

	void PhysicsWorld::Init()
	{
		// NOTE(Peter): Currently I only support Semi-Implicit Euler integration
		//				but in the future I'll implement Velocity Verlet and RK4
		//				so that I can compare them.
		m_BodyIntegrator = std::make_unique<SemiImplicitEulerIntegrator>();
		m_BroadPhase = std::make_unique<BroadPhase>();
	}

	void PhysicsWorld::Shutdown()
	{
		m_Bodies.clear();
	}

	void PhysicsWorld::Simulate(float InDeltaTime)
	{
		m_Stats = {};

		m_DeltaTime = InDeltaTime;

		for (auto& body : m_Bodies)
		{
			if (body->GetType() != EBodyType::Dynamic)
				continue;

			m_BodyIntegrator->IntegrateForceTorqueAndDrag(body->As<DynamicBody>(), m_Settings.Gravity, InDeltaTime);
		}

		ContactResolver resolver;

		m_BroadPhase->Execute(m_Bodies, InDeltaTime);

		const auto& collisionPairs = m_BroadPhase->GetCollisionPairs();

		m_Stats.NumCollisionPairs = collisionPairs.size();

		for (const auto& pair : collisionPairs)
		{
			auto& body0 = m_Bodies[pair.A];
			auto& body1 = m_Bodies[pair.B];

			if (body0->GetType() == EBodyType::Static && body1->GetType() == EBodyType::Static)
				continue;

			Shape* shape0 = body0->GetShape();
			Shape* shape1 = body1->GetShape();

			if (shape0 == nullptr || shape1 == nullptr)
				continue;

			bool isColliding = false;
			isColliding = CollisionChecker::TestSphereSphereCollision(body0->GetPosition(), static_cast<const SphereShape*>(shape0), body1->GetPosition(), static_cast<const SphereShape*>(shape1));

			if (isColliding)
			{
				resolver.GenerateContact(body0.get(), body1.get());
				m_Stats.NumContacts++;
			}
		}

		for (auto& body : m_Bodies)
		{
			if (body->GetType() != EBodyType::Dynamic)
				continue;

			m_BodyIntegrator->IntegrateVelocity(body->As<DynamicBody>(), InDeltaTime);
		}

		resolver.ResolveContacts();
	}

	Body* PhysicsWorld::CreateBody(const BodySettings& InSettings)
	{
		switch (InSettings.BodyType)
		{
		case EBodyType::Static:
		{
			auto body = std::make_unique<Body>();
			body->m_Position = InSettings.Position;
			body->m_Rotation = InSettings.Rotation;
			body->m_Shape = InSettings.CollisionShape;
			body->m_Friction = InSettings.Friction;
			body->m_Restitution = InSettings.Restitution;
			return m_Bodies.emplace_back(std::move(body)).get();
		}
		case EBodyType::Dynamic:
		{
			auto body = std::make_unique<DynamicBody>();
			body->m_Position = InSettings.Position;
			body->m_Rotation = InSettings.Rotation;
			body->m_Shape = InSettings.CollisionShape;
			body->m_InvMass = 1.0f / InSettings.Mass;
			body->m_Friction = InSettings.Friction;
			body->m_Restitution = InSettings.Restitution;
			body->UpdateInertiaTensor();
			return m_Bodies.emplace_back(std::move(body)).get();
		}
		}

		return nullptr;
	}

}
