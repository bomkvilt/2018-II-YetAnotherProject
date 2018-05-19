#pragma once
#ifndef MOVEMENT_STABILISATION_HPP
#define MOVEMENT_STABILISATION_HPP

#include <functional>
#include "MovementStates.hpp"


struct FMovementStabilisation
{
public:	//~~~~~~~~~~~~~~~~~~~| Member Define

	enum EClampMode {
		ECM_RAW, // 
		ECM_TRIM // 
		};

public:	//~~~~~~~~~~~~~~~~~~~| Costruction

	FMovementStabilisation();

	void BeginPlay();

public:	//~~~~~~~~~~~~~~~~~~~| Interface

	// 
	void Stabilisate(float DeltaTime);
	void Stabilisate(float DeltaTime, const FQuat& Rotation);

protected:	//~~~~~~~~~~~~~~~~| Members

	FMovementStates* states; // 
	BaseActorComponent*  target; // 

	FQuat   targetRotation;

public:

	void SetStates(FMovementStates* newStates);
	void SetTarget(BaseActorComponent*  newTarget);

protected:	//~~~~~~~~~~~~~~~|

	FVector StabilisationFunction(
		  const FQuat& DeltaRotation
		, const FVector& AngularSpeed
		, const FVector& Epsylon
		, float FallAngle
		, float DeltaSeconds
		);
};

#endif // !MOVEMENT_STABILISATION_HPP