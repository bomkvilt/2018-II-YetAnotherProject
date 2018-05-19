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

	// body type
	virtual ERigidBodyType GetBodyType() const override { return bodyType;       }
	virtual FCollisionRules GetRules()   const override { return collisionRules; }
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
	ERigidBodyType bodyType;
	BaseActorComponent* owner;
	FCollisionRules collisionRules;
};


#endif // !RIGID_BODY_HPP
