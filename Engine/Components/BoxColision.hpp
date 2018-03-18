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

	Vector3f GetExtends() const	{ return extends; }
	void SetExtends(Vector3f newExtends);

protected:

	/// >> 
	Vector3f extends;
	///
};