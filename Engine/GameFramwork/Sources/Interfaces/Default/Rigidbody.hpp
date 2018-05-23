#pragma once
#ifndef RIGID_BODY_HPP
#define RIGID_BODY_HPP


#include "Interfaces/IRigidBody.hpp"
#include "World.hpp"

struct FRigidBody : public IRigidBody
{
public:
	FRigidBody(BaseActorComponent* owner);

public: //~~~~~~~~~~~~~~| interface realisation
	/// << IRigidBody
	// owner
	virtual const BaseActorComponent* GetOwner() const  override { return owner;     }
	virtual       BaseActorComponent* GetOwner()        override { return owner;     }
	virtual void SetOwner(BaseActorComponent* newOwner) override { owner = newOwner; }

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
	BaseActorComponent* owner;
	bool bDynamic;
};


#endif // !RIGID_BODY_HPP
