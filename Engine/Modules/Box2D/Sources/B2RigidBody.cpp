#include "B2RigidBody.hpp"
#include "B2PhysicsScene.hpp"
#include "BaseActorComponent.hpp"
#include <iostream>

RigidBody::RigidBody(FShape shape, BaseActorComponent* owner, float mass, FVector inertia)
	: FRigidBody(owner)
	, rigidBody(nullptr)
	, fixture_r(nullptr)
	, fixture_s(nullptr)
	, density(0)
	, square (0)
{
	assert(owner);

	massData.center = b2Vec2_zero;
	massData.mass = 0;
	massData.I = 0;
	
	if (auto* scene = GetPhysicsScene<PhysicsScene>())
	{
		// rigind body and sensor
		b2BodyDef bodyDef;
		bodyDef.position.Set(0,0);
		rigidBody = scene->world.CreateBody(&bodyDef);

		// set them up
		shapeType = shape.type;
		SetExtents(shape.extents);
		SetInertia(inertia);
		SetMass(mass);

		// update collision states
		UpdateCollision();
		
		// register self
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
			b2Vec2() << transform.Location, 
			transform.Rotation.Z 
			);

		UpdateBody();
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

	// set new shape size and get new square
	switch (shapeType) {
	case EShapeType::eBox: 
		collision.SetAsBox(newExtents.X, newExtents.Y);
		collision.ComputeMass(&tmpMass, 1);
		square = tmpMass.mass;
		break;
	default: throw std::runtime_error("Unsupported shape type");
	}
	
	if (fixture_s) rigidBody->DestroyFixture(fixture_s);
	if (fixture_r) rigidBody->DestroyFixture(fixture_r);

	b2FixtureDef def;
	def.shape   = &collision;
	def.density = density;
	/// rigid body
	fixture_r = rigidBody->CreateFixture(&def);
	rigidBody->SetMassData(&massData);
	/// sensor
	def.isSensor = true; // make the sensor
	def.density  = 0;	 // not  affecting
	def.friction = 0;	 // the  body
	fixture_s = rigidBody->CreateFixture(&def);

	UpdateBody();
	UpdateCollision();
}

void RigidBody::SetBodyType(ERigidBodyType newType)
{
	bodyType = newType;

	UpdateBody();
}

void RigidBody::SetMass(float newMass)
{
	density  = newMass / square;
	collision.ComputeMass(&massData, density);
	rigidBody->SetMassData(&massData);

	UpdateBody();
}

void RigidBody::SetInertia(FVector newInertia)
{
	massData.I = newInertia.Z;
	rigidBody->SetMassData(&massData);

	UpdateBody();
}

float RigidBody::GetMass() const
{
	return massData.mass;
}

FVector RigidBody::GetInertia() const
{
	return FVector(0, 0, massData.I);
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

void RigidBody::SetCollisionRules(const FCollisionRules& newRules)
{
	collisionRules = newRules;
	UpdateCollision();
}

b2Transform RigidBody::GetTransform() const
{
	if (owner)
	{
		return b2Transform() << ~owner->GetRelativeTransform();
	}
	return b2Transform();
}

void RigidBody::UpdateBody()
{
	if (bodyType != ERigidBodyType::eStatic)
	{
		bodyType = (!massData.mass || !massData.I) 
			? ERigidBodyType::eKinematic
			: ERigidBodyType::eDynamic
			;
	}

	rigidBody->SetMassData(&massData);

	switch (bodyType) 
	{
	case ERigidBodyType::eStatic:	 rigidBody->SetType(b2_staticBody   ); break;
	case ERigidBodyType::eKinematic: rigidBody->SetType(b2_kinematicBody); break;
	case ERigidBodyType::eDynamic:   rigidBody->SetType(b2_dynamicBody  ); break;
	default: throw std::runtime_error("unknown body type");
	}
}

void RigidBody::UpdateCollision()
{
	auto filter_r = fixture_r->GetFilterData();
	auto filter_s = fixture_s->GetFilterData();

	filter_r.categoryBits = 1 << collisionRules.category;
	filter_s.categoryBits = 1 << collisionRules.category;

	for (int i = 0; i < FCollisionRules::maxCategoryCount; ++i)
	{
		SetupCollision(i, filter_r, filter_s);
	}

	fixture_r->SetFilterData(filter_r);
	fixture_s->SetFilterData(filter_s);
}

void RigidBody::SetupCollision(int bit, b2Filter& filter_r, b2Filter& filter_s)
{
	auto type = collisionRules.rules[bit];
	switch (type)
	{
	case ECollisiontype::eCollide: filter_r.maskBits |=  (1 << bit); filter_s.maskBits &= ~(1 << bit); break;
	case ECollisiontype::eOverlap: filter_r.maskBits &= ~(1 << bit); filter_s.maskBits |=  (1 << bit); break;
	case ECollisiontype::eIgnore : filter_r.maskBits &= ~(1 << bit); filter_s.maskBits &= ~(1 << bit); break;
	default: throw std::runtime_error("unsupported value");
	}
}
