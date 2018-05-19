#include "MovementStabilisation.hpp"
#include <iostream>

FMovementStabilisation::FMovementStabilisation()
	: states(nullptr)
	, target(nullptr)
{
	targetRotation = FQuat::Identity;
}

void FMovementStabilisation::BeginPlay()
{
}

void FMovementStabilisation::Stabilisate(float DeltaTime)
{
	Stabilisate(DeltaTime, targetRotation);
}

void FMovementStabilisation::Stabilisate(float DeltaTime, const FQuat& Rotation)
{
	if (!states && target && DeltaTime) return;
	
	FQuat   currQuat = states->Transform().Rotation;
	FQuat   delta    = ~Rotation * currQuat;    // get delta betwin target and current rotation
	FVector rotation = delta.GetEulerAngles();  // convert it to euler
	FVector epsilon  = states->NormalEpsilon(); // maximal epsilon
	FVector speed    = states->Omega();         // current omega
	float fallAngle  = states->CriticalAngle();

	FVector control = StabilisationFunction(
		  delta
		, speed
		, epsilon
		, fallAngle
		, DeltaTime 
		);
	states->Torque() += epsilon * control;
}

void FMovementStabilisation::SetStates(FMovementStates* newStates)
{
	states = newStates;
}

void FMovementStabilisation::SetTarget(BaseActorComponent* newTarget)
{
	target = newTarget;
}

FVector FMovementStabilisation::StabilisationFunction(
	  const FQuat& DeltaRotation
	, const FVector& AngularSpeed
	, const FVector& Epsylon
	, float FallAngle
	, float DeltaSeconds )
{
	auto f0 = DeltaRotation.GetEulerAngles();
	auto w0 = AngularSpeed;
	auto em = Epsylon;
	auto fm = FallAngle;

	auto k2 = em / fm;
	auto k1 = Sqrt(k2);
	auto e = -k1*w0 - k2*f0;

	auto factor = e / em;
	if (std::isnan(factor.X)) factor.X = 0;
	if (std::isnan(factor.Y)) factor.Y = 0;
	if (std::isnan(factor.Z)) factor.Z = 0;

	return Clamp(factor, -1, 1);
}