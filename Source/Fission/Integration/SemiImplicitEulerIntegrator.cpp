#include "FissionPCH.hpp"
#include "SemiImplicitEulerIntegrator.hpp"

namespace Fission {

	void SemiImplicitEulerIntegrator::IntegrateForceTorqueAndDrag(DynamicBody* InBody, const Math::FVec3& InGravity, float InDeltaTime)
	{
		Math::FVec3 LinearVelocity = InBody->GetLinearVelocity();
		LinearVelocity += (InGravity + InBody->GetAccumulatedForce() * InBody->GetInverseMass()) * InDeltaTime;
		InBody->SetLinearVelocity(LinearVelocity);
	}

	void SemiImplicitEulerIntegrator::IntegrateVelocity(DynamicBody* InBody, float InDeltaTime)
	{
		const auto& Position = InBody->GetPosition();
		InBody->SetPosition(Position + InBody->GetLinearVelocity() * InDeltaTime);

		// Integrate Angular Velocity
		auto PositionCOM = InBody->GetWorldSpaceCenterOfMass();
		auto RelativePositionCOM = Position - PositionCOM;
		auto RotationMat = Math::Mat3x3::FromQuat(InBody->GetRotation());
		auto InertiaTensor = RotationMat * InBody->GetShape()->GetInertiaTensor() * RotationMat.Transposed();

		const auto& AngularVelocity = InBody->GetAngularVelocity();

		auto Alpha = InertiaTensor.Inversed() * (AngularVelocity.Cross(InertiaTensor * AngularVelocity));
		InBody->SetAngularVelocity(AngularVelocity + Alpha);

		// Update Rotation
		auto DeltaAngle = AngularVelocity * InDeltaTime;
		Math::FQuat DeltaRotation(DeltaAngle, DeltaAngle.Length());
		const auto& Rotation = InBody->GetRotation();
		InBody->SetRotation((DeltaRotation * Rotation).Normalized());

		PositionCOM = InBody->GetWorldSpaceCenterOfMass();

		// Update Position to account for new rotation
		InBody->SetPosition(PositionCOM + DeltaRotation * RelativePositionCOM);
	}

}
