#include "Actor.hpp"

Actor::Actor() : rootComponent(nullptr)
{}


//~~~~~~~~~~~~~~~~~~~~~~| << proxi functions

void Actor::AddForce    (const FVector& force, ESpaceType space) { if (rootComponent) rootComponent->AddForce (force, space); }
void Actor::AddTorque   (const FVector& torue, ESpaceType space) { if (rootComponent) rootComponent->AddTorque(torue, space); }

// transform

void        Actor::SetComponentTransform    (FTransform newTransform) { if (rootComponent) rootComponent->SetComponentTransform(newTransform); }
void        Actor::SetRelativeTransform	    (FTransform newTransform) { if (rootComponent) rootComponent->SetRelativeTransform (newTransform); }
FTransform  Actor::GetComponentTransform    () const                  { if (rootComponent) return rootComponent->GetComponentTransform(); return FTransform::Identity; }
FTransform  Actor::GetRelativeTransform     () const                  { if (rootComponent) return rootComponent->GetRelativeTransform();  return FTransform::Identity; }

// location

void        Actor::SetComponentLocation     (FVector newLocation)    { if (rootComponent) rootComponent->SetComponentLocation(newLocation); }
void        Actor::SetRelativeLocation      (FVector newLocation)    { if (rootComponent) rootComponent->SetRelativeLocation (newLocation); }
FVector	    Actor::GetComponentLocation     () const                 { if (rootComponent) return rootComponent->GetComponentLocation(); return FVector::ZeroVector; }
FVector	    Actor::GetRelativeLocation      () const                 { if (rootComponent) return rootComponent->GetRelativeLocation();  return FVector::ZeroVector; }

// rotation

void        Actor::SetComponentRotation     (FQuat newRotation)	     { if (rootComponent) rootComponent->SetComponentRotation(newRotation); }
void        Actor::SetRelativeRotation      (FQuat newRotation)	     { if (rootComponent) rootComponent->SetRelativeRotation (newRotation); }
FQuat       Actor::GetComponentRotation     () const                 { if (rootComponent) return rootComponent->GetComponentRotation(); return FQuat::Identity; }
FQuat       Actor::GetRelativeRotation      () const                 { if (rootComponent) return rootComponent->GetRelativeRotation();  return FQuat::Identity; }

// transform

void        Actor::AddTransform	            (FTransform delta, ESpaceType space) { if (rootComponent) rootComponent->AddTransform        (delta, space); }
void        Actor::AddComponentLocation     (FVector    delta, ESpaceType space) { if (rootComponent) rootComponent->AddComponentLocation(delta, space); }
void        Actor::AddComponentRotation     (FQuat      delta, ESpaceType space) { if (rootComponent) rootComponent->AddComponentRotation(delta, space); }

//~~~~~~~~~~~~~~~~~~~~~~| >> proxi functions

void Actor::SetRootComponent(ActorComponent* newRoot)
{
	if (rootComponent)
	{
		rootComponent->AttachTo(newRoot);
	}
	rootComponent = newRoot;
}