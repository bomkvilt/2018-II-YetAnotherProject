#include "B2PhysicsScene.hpp"
#include "BaseActorComponent.hpp"
#include "Actor.hpp"

PhysicsScene::PhysicsScene()
	: world(b2Vec2(0, -10))
{
	world.SetContactListener(&contactListener);
}

void PhysicsScene::Update(float deltaTime)
{
	world.Step(deltaTime, 6, 4);

	SyncRigidBodies();
	ProcessCollisions();
}

void PhysicsScene::RegisterBody(IRigidBody* ibody)
{
	if (auto* body = dynamic_cast<RigidBody*>(ibody))
	{
		rigidBodies.emplace(body);
		body2body[body->rigidBody] = body;
	}
}

void PhysicsScene::UnregisterBody(IRigidBody* ibody)
{
	if (auto* body = dynamic_cast<RigidBody*>(ibody))
	{
		rigidBodies.erase(body);
		body2body.erase(body->rigidBody);
	}
}

void PhysicsScene::SyncRigidBodies()
{
	for (auto body : rigidBodies)
	{
		if (body)
		{
			body->Sync();
		}
	}
}

void PhysicsScene::ProcessCollisions()
{
	// OnCollisionBegin
	for (auto& pair  : contactListener.newConacts)
	for (auto& pair2 : pair.second)
	{
		auto contact = pair2.second;
		
		if (body2body.count(contact.body ) // if the bodies are registered
		 && body2body.count(contact.other))
		{
			RigidBody* target = body2body[contact.body ];
			RigidBody* other  = body2body[contact.other];

			if (target && other)
			{
				FHit hit;
				hit.component      = target->GetOwner();
				hit.otherComponent = other ->GetOwner();
		
				if (hit.component && hit.otherComponent)
				{
					hit.actor = hit.component     ->GetOwner();
					hit.other = hit.otherComponent->GetOwner();

					hit.Location  = contact.Location;
					hit.HitNormal = contact.HitNormal;

					hit.component->OnCollisionEnter.Broadcast(
						hit.other,
						hit.otherComponent,
						hit
						);
					continue;
				}
			}
		}
		else throw std::runtime_error("Unregistered body is used");
	}

	// OnCollisionEnd
	for (auto& pair  : contactListener.FinishedContacts)
	for (auto& other : pair.second)
	{
		auto* body = pair.first;

		if (body2body.count(body )
		 && body2body.count(other))
		{
			RigidBody* target    = body2body[body ];
			RigidBody* otherBody = body2body[other];

			if (target && otherBody)
			{
				auto* component      = target   ->GetOwner();
				auto* otherComponent = otherBody->GetOwner();
		
				if (component && otherComponent)
				{
					auto* actor = component     ->GetOwner();
					auto* other = otherComponent->GetOwner();

					component->OnCollisionExit.Broadcast(
						other, otherComponent
						);
					continue;
				}
			}
		}
		else throw std::runtime_error("Unregistered body is used");
	}

	contactListener.Flush();
}
