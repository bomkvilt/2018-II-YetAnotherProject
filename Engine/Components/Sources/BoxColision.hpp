#ifndef BOX_COLISION_HPP
#define BOX_COLISION_HPP

#include "ActorComponent.hpp"

class BoxColision : public ActorComponent
{
	GENERATED_BODY(BoxColision, ActorComponent);
public:

	BoxColision();

public:

	FVector GetExtents() const	
	{ 
		return extents; 
	}

	void SetExtents(FVector newExtents);

protected:

	/// >> 
	FVector extents;
	///
};

#endif