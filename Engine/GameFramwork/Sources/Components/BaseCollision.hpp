#pragma once
#ifndef BASE_COLLISION_HPP
#define BASE_COLLISION_HPP

#include "ActorComponent.hpp"


class BaseCollision : public ActorComponent
{
	GENERATED_BODY(BaseCollision, ActorComponent);

public:
	BaseCollision(EShapeType shape);

public:
	FVector GetExtents() const;
	void SetExtents(FVector newExtents);

protected:
	/// >> 
	FVector extents;
	///
};


#endif //!BASE_COLLISION_HPP