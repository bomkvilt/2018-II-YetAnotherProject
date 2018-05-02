#include "B2RigidBody.hpp"
#include "B2PhysicsScene.hpp"
#include "ActorComponent.hpp"
#include <iostream>

RigidBody::RigidBody(FShape shape, ActorComponent* owner, float mass, FVector inertia)
	: FRigidBody(owner)
	, fixture(nullptr)
	, density(0)
	, square(0)
{
	assert(owner);

	if (auto* scene = GetPhysicsScene<PhysicsScene>())
	{
		b2BodyDef bodyDef;
		bodyDef.position.Set(0,0);
		rigidBody = scene->world.CreateBody(&bodyDef);

		shapeType = shape.type;
		SetExtents(shape.extents);
		SetInertia(inertia);
		SetMass(mass);
		
		scene->RegisterBody(this);
	}
}

RigidBody::~RigidBody()
{
	if (auto* scene = GetPhysicsScene<PhysicsScene>())
	{
		scene->UnregisterBody(this);
		scene->world.DestroyBody(rigidBody);
		rigidBody = nullptr;
	}
}

void RigidBody::Update()
{
	if (owner && rigidBody)
	{
		rigidBody->SetType(b2_kinematicBody);
		FTransform transform = owner->GetComponentTransform();
		rigidBody->SetTransform(
			b2Vec2() << transform.Location
			, transform.Rotation.Z
			);
		UpdateState();
	}
}

void RigidBody::Sync()
{
	if (owner && rigidBody && bDynamic)
	{
		FTransform newTransform = FTransform() << rigidBody->GetTransform();
		owner->SetComponentTransform(newTransform, true, false);
	}
}

void RigidBody::AddForce(const FVector& force)
{
	if (rigidBody && force != FVector::ZeroVector)
	{
		std::cout << force.ToString() << std::endl;
		rigidBody->ApplyForceToCenter(
			b2Vec2() << force
			, true 
			);
	}
}

void RigidBody::AddTorque(const FVector& torque)
{
	if (rigidBody)
	{
		rigidBody->ApplyTorque(
			torque.Z, 
			true 
			);
	}
}

void RigidBody::AddImpulce(const FVector& impulce)
{
	if (rigidBody)
	{
		std::cout << impulce.ToString() << std::endl;
		rigidBody->ApplyLinearImpulseToCenter(
			b2Vec2() << impulce
			, true
			);
	}
}

void RigidBody::AddKineticMomement(const FVector& moment)
{
	if (rigidBody)
	{
		rigidBody->ApplyAngularImpulse(
			moment.Z
			, true
			);
	}
}

void RigidBody::SetMass(float newMass)
{
	bDynamic = newMass;
	density  = newMass / square;
	collision.ComputeMass(&massData, density);
	rigidBody->SetMassData(&massData);

	UpdateState();
}

void RigidBody::SetInertia(FVector newInertia)
{
	bDynamic   = newInertia.Z && bDynamic;
	massData.I = newInertia.Z;
	rigidBody->SetMassData(&massData);
}

float RigidBody::GetMass() const
{
	return massData.mass;
}

FVector RigidBody::GetInertia() const
{
	return FVector(0, 0, massData.I);
}

void RigidBody::SetExtents(FVector newExtents)
{
	b2MassData tmpMass;

	switch (shapeType) 
	{
	case EShapeType::eBox: 
		collision.SetAsBox(newExtents.X, newExtents.Y);
		collision.ComputeMass(&tmpMass, 1);
		square = tmpMass.mass;
		break;
	default: assert(false);
	}

	if (fixture)
	{
		rigidBody->DestroyFixture(fixture);
	}

	fixture = rigidBody->CreateFixture(
		&collision,
		density
		);
	rigidBody->SetMassData(&massData);

	UpdateState();
}

FVector RigidBody::GetVelocity() const
{
	if (rigidBody)
	{
		return FVector() << rigidBody->GetLinearVelocity();
	}
	return FVector::ZeroVector;
}

void RigidBody::SetVelocity(const FVector& newVelocity)
{
	if (rigidBody)
	{
		rigidBody->SetLinearVelocity(b2Vec2() << newVelocity);
	}
}

FVector RigidBody::GetOmega() const
{
	if (rigidBody)
	{
		return FVector(0, 0, rigidBody->GetAngularVelocity());
	}
	return FVector::ZeroVector;
}

void RigidBody::SetOmega(const FVector& newOmega)
{
	if (rigidBody)
	{
		rigidBody->SetAngularVelocity(newOmega.Z);
	}
}

b2Transform RigidBody::GetTransform() const
{
	if (owner)
	{
		return b2Transform() << ~owner->GetRelativeTransform();
	}
	return b2Transform();
}

void RigidBody::UpdateState()
{
	if (bDynamic)
	{
		rigidBody->SetMassData(&massData);
		rigidBody->SetType(b2_dynamicBody);
		rigidBody->SetAwake(true);
	}
	else
	{
		rigidBody->SetType(b2_kinematicBody);
		rigidBody->SetAwake(true);
	}
}
