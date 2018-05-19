#include "B2RigidBody.hpp"
#include "B2PhysicsScene.hpp"
#include "BaseActorComponent.hpp"
#include <iostream>

RigidBody::RigidBody(FShape shape, BaseActorComponent* owner, float mass, FVector inertia)
	: FRigidBody(owner)
	, fixture(nullptr)
	, density(0)
	, square(0)
{
	assert(owner);

	massData.center = b2Vec2_zero;
	massData.mass   = 0;
	massData.I      = 0;
	
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
	}
}

void RigidBody::Update()
{
	if (owner && rigidBody)
	{
		rigidBody->SetType(b2_kinematicBody);
		FTransform transform = owner->GetComponentTransform();
		rigidBody->SetTransform (
			b2Vec2() << transform.Location
			, transform.Rotation.Z
			);
		UpdateState();
	}
}

void RigidBody::Sync()
{
	if (owner && rigidBody)
	{
		FTransform prevTransfotm = owner->GetComponentTransform();
		FTransform newTransform  = prevTransfotm << rigidBody->GetTransform();
		owner->SetComponentTransform(newTransform, true, false);
	}
}

void RigidBody::AddForce(const FVector& force)
{
	if (rigidBody && force != FVector::ZeroVector)
	{
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
		&collision
		, density
		);
	rigidBody->SetMassData(&massData);

	UpdateState();
}

void RigidBody::SetBodyType(ERigidBodyType newType)
{
	bodyType = newType;

	UpdateState();
}

void RigidBody::SetMass(float newMass)
{
	density  = newMass / square;
	collision.ComputeMass(&massData, density);
	rigidBody->SetMassData(&massData);

	UpdateState();
}

void RigidBody::SetInertia(FVector newInertia)
{
	massData.I = newInertia.Z;
	rigidBody->SetMassData(&massData);

	UpdateState();
}

float RigidBody::GetMass() const
{
	return massData.mass;
}

FVector RigidBody::GetInertia() const
{
	return FVector(0, 0, massData.I);
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
		auto rads = rigidBody->GetAngularVelocity();
		return FVector(0, 0, RAD2DEG(rads));
	}
	return FVector::ZeroVector;
}

void RigidBody::SetOmega(const FVector& newOmega)
{
	if (rigidBody)
	{
		auto degs = newOmega.Z;
		rigidBody->SetAngularVelocity(DEG2RAD(degs));
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
	if (bodyType != ERigidBodyType::eIgnore && bodyType != ERigidBodyType::eStatic)
	{
		bodyType = (!massData.mass || !massData.I) 
			? ERigidBodyType::eKinematic
			: ERigidBodyType::eDynamic
			;
	}

	auto data = fixture->GetFilterData();
	switch (bodyType) {
	case ERigidBodyType::eStatic:	 data.categoryBits = 1; data.maskBits = 1; rigidBody->SetMassData(&massData); rigidBody->SetType(b2_staticBody   ); break;
	case ERigidBodyType::eKinematic: data.categoryBits = 1; data.maskBits = 1; rigidBody->SetMassData(&massData); rigidBody->SetType(b2_kinematicBody); break;
	case ERigidBodyType::eDynamic:   data.categoryBits = 1; data.maskBits = 1; rigidBody->SetMassData(&massData); rigidBody->SetType(b2_dynamicBody  ); break;
	case ERigidBodyType::eIgnore:    data.categoryBits = 1; data.maskBits = 0; rigidBody->SetMassData(&massData); rigidBody->SetType(b2_staticBody   ); break;
	default: throw std::runtime_error("unknown dody type");
	}
	fixture->SetFilterData(data);
}
