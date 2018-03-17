#pragma once


#include "../Core/ActorComponent.hpp"

class BoxColision : public ActorComponent
{
	using Super = ActorComponent;

public:

	BoxColision();

public:

	virtual void Tick(float DeltaTime, ETickType type) override;

public:

	btVector3 GetExtends() const	{ return extends; }
	void SetExtends(btVector3 newExtends);

protected:

	/// >> 
	btVector3 extends;
	///
};