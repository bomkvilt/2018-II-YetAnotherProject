#include "MovementComponent.hpp"
#include "Actor.hpp"

MovementComponent::MovementComponent()
	: updatedComponent(nullptr)
{
	prePhysicsTick .BindFunction(this, owner, &MovementComponent::PrePhysicsTick );
	postPhysicsTick.BindFunction(this, owner, &MovementComponent::PostPhysicsTick);

	prePhysicsTick .tickType = ETickType::ePrePhysics;
	postPhysicsTick.tickType = ETickType::ePostPhysics;

	stabilisation.SetStates(&states);
}

void MovementComponent::OnConstructed()
{
	Super::OnConstructed();
	RegisterTick(prePhysicsTick );
	RegisterTick(postPhysicsTick);
}

void MovementComponent::OnEndPlay()
{
	Super::OnEndPlay();
	UnregisterTick(prePhysicsTick );
	UnregisterTick(postPhysicsTick);
}

void MovementComponent::PrePhysicsTick(float DeltaTime, ETickType type)
{
	if (states.flags.bUseStabilisation) 
	{
		stabilisation.Stabilisate(DeltaTime);
	}

	if (updatedComponent)
	{
		updatedComponent->AddForce          (GetResultForce()         , ESpaceType::eLocal);
		updatedComponent->AddTorque         (GetResultTorque()        , ESpaceType::eLocal);
		updatedComponent->AddImpulce        (GetResultImpulce()       , ESpaceType::eLocal);
		updatedComponent->AddKineticMomement(GetResultAngularImpulce(), ESpaceType::eLocal);
	}
}

void MovementComponent::PostPhysicsTick(float DeltaTime, ETickType type)
{
	states.NextFrame(DeltaTime);    /// Here we start new frame
	AbsorbTelemetry();              /// Take a telemetry
	states.UpdateStates(DeltaTime); /// Update accelerations
}

void MovementComponent::SetTrackingComponent(BaseActorComponent* target)
{
	updatedComponent = target;
	stabilisation.SetTarget(target);
}

void MovementComponent::AddLenearInput (FVector WorldVector, ESpaceType space) { AddState(WorldVector, states.LinearInput() , space); }
void MovementComponent::AddAngularInput(FVector WorldVector, ESpaceType space) { AddState(WorldVector, states.AngularInput(), space); }
void MovementComponent::AddForce       (FVector Force      , ESpaceType space) { AddState(Force,       states.Force()       , space); }
void MovementComponent::AddTorque      (FVector Torque     , ESpaceType space) { AddState(Torque,      states.Torque()      , space); }

void MovementComponent::AddState(const FVector& Value, FVector& Target, ESpaceType space) const
{
	if(updatedComponent)
	{
		Target += updatedComponent->SpaceToLocal(Value, space);
	}
}

FVector MovementComponent::GetResultForce() const
{
	if (updatedComponent)
	{
		FVector force;

		switch (states.CurrentMode().ImpactType)
		{
		case EMovementImpactType::Acceleration:        
			force += states.LinearInput() 
			      *  states.NormalAcceleration() 
			      *  updatedComponent->GetMass();
		case EMovementImpactType::Force: 
			force += states.LinearInput() 
			      *  states.NormalForce();
		}
		return force + states.Force();
	}
	return FVector::ZeroVector;
}

FVector MovementComponent::GetResultTorque() const
{
	if (updatedComponent)
	{
		FVector torque;

		switch (states.CurrentMode().ImpactType)
		{
		case EMovementImpactType::Acceleration:        
			torque += states.AngularInput() 
			       *  states.NormalEpsilon() 
			       *  updatedComponent->GetInertia();
		case EMovementImpactType::Force: 
			torque += states.AngularInput() 
			       *  states.NormalTorque();
		}
		return torque + states.Torque();
	}
	return FVector::ZeroVector;
}

FVector MovementComponent::GetResultImpulce() const
{
	FVector impulce;

	if (states.ImpactType() == EMovementImpactType::Impulce)
	{
		impulce += states.LinearInput()
		        *  states.NormalImpulce();
	}
	return impulce += states.Impulce();
}

FVector MovementComponent::GetResultAngularImpulce() const
{
	FVector impulce;

	if (states.ImpactType() == EMovementImpactType::Impulce)
	{
		impulce += states.LinearInput()
		        *  states.NormalAngularImpulce();
	}
	return impulce += states.AngularImpulce();
}

void MovementComponent::AbsorbTelemetry()
{
	if (auto* t = updatedComponent)
	{
		states.Transform() = t->GetComponentTransform();
		states.Velocity()  = t->SpaceToLocal(t->GetVelocity(), ESpaceType::eWorld);
		states.Omega()     = t->SpaceToLocal(t->GetOmega()   , ESpaceType::eWorld);
	}
}
