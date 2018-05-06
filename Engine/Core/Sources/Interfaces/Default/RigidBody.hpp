#pragma once
#ifndef RIGID_BODY_HPP
#define RIGID_BODY_HPP

#include "../IRigidBody.hpp"
#include "World.hpp"

struct FRigidBody : public IRigidBody
{
public:
	FRigidBody(ActorComponent* owner);

public: //~~~~~~~~~~~~~~| interface realisation
	/// << IRigidBody
	// owner
	virtual const ActorComponent* GetOwner() const  override { return owner;     }
	virtual       ActorComponent* GetOwner()        override { return owner;     }
	virtual void SetOwner(ActorComponent* newOwner) override { owner = newOwner; }

	// bDynamic
	virtual bool IsDinamic() const override { return bDynamic; }
	/// >>

protected: //~~~~~~~~~~~| helper function

	template<class _ST> 
	_ST* GetPhysicsScene()
	{
		if (auto* world = GetWorld())
		{
			return dynamic_cast<_ST*>(world->GetPhysicsScene());
		}
		return nullptr;
	}

	World* GetWorld();

protected:
	ActorComponent* owner;
	bool bDynamic;
};


#endif // !RIGID_BODY_HPP
