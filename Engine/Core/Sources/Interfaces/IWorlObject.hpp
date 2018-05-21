#pragma once
#ifndef IWORLD_OBJECT_HPP
#define IWORLD_OBJECT_HPP

#include "Common.hpp"


struct IWorldObject
{
	virtual ~IWorldObject() {}

	/// transform

	virtual FTransform GetComponentTransform() const = 0;
	virtual FTransform GetRelativeTransform () const = 0;
	virtual void SetComponentTransform(FTransform newTransform) = 0;
	virtual void SetRelativeTransform (FTransform newTransform) = 0;

	/// location

	virtual FVector GetComponentLocation() const = 0;
	virtual FVector GetRelativeLocation () const = 0;
	virtual void SetComponentLocation(FVector newLocation) = 0;
	virtual void SetRelativeLocation (FVector newLocation) = 0;

	/// rotation

	virtual FQuat GetComponentRotation() const = 0;
	virtual FQuat GetRelativeRotation () const = 0;
	virtual void SetComponentRotation(FQuat newRotation) = 0;
	virtual void SetRelativeRotation (FQuat newRotation) = 0;

	/// add

	virtual void AddTransform        (FTransform delta, ESpaceType space = ESpaceType::eWorld) = 0;
	virtual void AddComponentLocation(FVector	 delta, ESpaceType space = ESpaceType::eWorld) = 0;
	virtual void AddComponentRotation(FQuat      delta, ESpaceType space = ESpaceType::eWorld) = 0;

};


#endif // IWORLD_OBJECT_HPP
