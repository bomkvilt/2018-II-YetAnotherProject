#pragma once
#ifndef MOVEMENT_COMPONENT_HPP
#define MOVEMENT_COMPONENT_HPP

#include "MovementComponentUtiles/MovementStates.hpp"
#include "MovementComponentUtiles/MovementStabilisation.hpp"


class MovementComponent : public ActorModule
{
	GENERATED_BODY(MovementComponent, ActorModule);

public:	//~~~~~~~~~~~~~~~~~~~| >> 

	MovementComponent();

	virtual void OnConstructed() override;
	virtual void OnEndPlay()     override;

	virtual void PrePhysicsTick (float DeltaTime, ETickType type);
	virtual void PostPhysicsTick(float DeltaTime, ETickType type);

public: //~~~~~~~~~~~~~~~~~~~| >> Interface
public:	//~~~~~~~~~| Initialisaiton |

	virtual void SetTrackingComponent(BaseActorComponent* target);

public:	//~~~~~~~~~| Controll |
	virtual void AddLenearInput (FVector WorldVector, ESpaceType space = ESpaceType::eLocal);
	virtual void AddAngularInput(FVector WorldVector, ESpaceType space = ESpaceType::eLocal);
	virtual void AddForce       (FVector Force      , ESpaceType space = ESpaceType::eLocal);
	virtual void AddTorque      (FVector Torque     , ESpaceType space = ESpaceType::eLocal);

public:	//~~~~~~~~~~~~~~~~~~~| >> Submodules

	FMovementStates        states;
	FMovementStabilisation stabilisation;

protected:	//~~~~~~~~~~~~~~~~| >> 
protected:  //~~~~~| tracking component |
	
	BaseActorComponent* updatedComponent;

protected:	//~~~~~| Secondary Ticks |

	FTickFunction prePhysicsTick;
	FTickFunction postPhysicsTick;

protected:	//~~~~~~~~~~~~~~~| >> Heplers

	void AddState(const FVector& Value, FVector& Target , ESpaceType space) const;
	/// << result impact
	FVector GetResultForce() const;
	FVector GetResultTorque() const;
	FVector GetResultImpulce() const;
	FVector GetResultAngularImpulce() const;

	void AbsorbTelemetry();
};


#endif //!MOVEMENT_COMPONENT_HPP