#include "FissionPCH.hpp"
#include "SemiImplicitEulerIntegrator.hpp"

namespace Fission {

	void SemiImplicitEulerIntegrator::IntegrateForceTorqueAndDrag(DynamicBody* InBody, const Math::FVec3& InGravity, float InDeltaTime)
	{
		Math::FVec3 linearVelocity = InBody->GetLinearVelocity();
		linearVelocity += (InGravity + InBody->GetAccumulatedForce() * InBody->GetInverseMass()) * InDeltaTime;
		InBody->SetLinearVelocity(linearVelocity);
	}

	void SemiImplicitEulerIntegrator::IntegrateVelocity(DynamicBody* InBody, float InDeltaTime)
	{
		const auto& position = InBody->GetPosition();
		InBody->SetPosition(position + InBody->GetLinearVelocity() * InDeltaTime);

		// Integrate Angular Velocity
		//auto positionCOM = InBody->GetWorldSpaceCenterOfMass();
		//auto relativePositionCOM = position - positionCOM;
		//auto rotationMat = Math::Mat3x3::FromQuat(InBody->GetRotation());
		//auto inertiaTensor = rotationMat * InBody->GetShape()->GetInertiaTensor() * rotationMat.Transposed();

		//const auto& angularVelocity = InBody->GetAngularVelocity();

		//auto alpha = inertiaTensor.Inversed() * (angularVelocity.Cross(inertiaTensor * angularVelocity));
		//InBody->SetAngularVelocity(angularVelocity + alpha);

		//// Update Rotation
		//auto deltaAngle = angularVelocity * InDeltaTime;
		//Math::FQuat deltaRotation(deltaAngle, deltaAngle.Length());
		//const auto& rotation = InBody->GetRotation();
		//InBody->SetRotation((deltaRotation * rotation).Normalized());

		//positionCOM = InBody->GetWorldSpaceCenterOfMass();

		//// Update Position to account for new rotation
		//InBody->SetPosition(positionCOM + deltaRotation * relativePositionCOM);
	}

}
