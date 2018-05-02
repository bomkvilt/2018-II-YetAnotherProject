#include "Pendulum.hpp"

Pendulum::Pendulum()
	: pid(4, 0, 3)
{
	base = CreateSubcomponent<BoxColision>("Base");
	base->SetExtents(FVector(0.6f, 0.6f, 0.6f));
	base->GetRigidBody()->SetMass(0);

	floor = CreateSubcomponent<BoxColision>("floor");
	floor->AddComponentLocation(FVector(0, -8, 0));
	floor->SetExtents(FVector(5, 0.1f, 5));
	floor->GetRigidBody()->SetMass(0);

	joint = CreateSubcomponent<JointComponent>("joint");
	joint->SetConstraint(FConstraintType::MakeRotation(eYaw));
	joint->AttachTo(base);

	pendulum = CreateSubcomponent<BoxColision>("Pendulum");
	pendulum->AddComponentLocation(FVector(2.5f, 0, 0), eParent);
	pendulum->SetExtents(FVector(1.5f, 0.2f, 0.2f));
	pendulum->GetRigidBody()->SetMass(10);
	pendulum->AttachTo(joint);

	target = CreateSubcomponent<BoxColision>("Target");
	target->AddComponentLocation(FVector(5, 0, 0), eParent);
	target->AddComponentRotation(FQuat(0, 0, -90), eParent);
	target->SetExtents(FVector(0.5f, 0.1f, 0.1f));
	target->GetRigidBody()->SetMass(0);
	target->AttachTo(joint);

	jumper = CreateSubcomponent<BoxColision>("Target");
	jumper->AddComponentLocation(FVector(-1, -7, 0), eParent);
	jumper->SetExtents(FVector(0.1f, 0.1f, 0.1f));
	jumper->GetRigidBody()->SetMass(1);

	cam = CreateSubcomponent<CameraComponent>("Camera");
	cam->AddComponentLocation(FVector(0, -5, 40), eParent);
	cam->AddComponentRotation(FQuat(0, 90, 0), eLocal);
	cam->SetAutoregister(true);
}

void Pendulum::OnBeginPlay()
{
	Super::OnBeginPlay();

	jumper->OnCollisionEnter.Bind(this, [&](Actor*, ActorComponent*, FHit)
	{
		std::cout << "Collision Enter" << std::endl;

		jumper->AddComponentLocation(FVector2(0, 2));
	});

	jumper->OnCollisionEnter.Bind(this, [&](Actor*, ActorComponent*, FHit)
	{
		std::cout << "Collision Exit" << std::endl;
	});
}

void Pendulum::Tick(float DeltaTime, ETickType type)
{
	Super::Tick(DeltaTime, type);

	dt = DeltaTime;

	auto tr = target->GetComponentRotation();
	auto cr = pendulum->GetComponentRotation();
	auto delta = ~cr * tr;

	const float control = pid.GetValue(delta.Z, dt);
	const float M0 = 150;
	const float M = M0 * control;
	pendulum->AddTorque(FVector(0, 0, M), eParent);
}

void Pendulum::SetupInput(EventBinder* binder)
{
	Super::SetupInput(binder);
	binder->BindAxis("Bvd", this, &Pendulum::Cam_1);
	binder->BindAxis("Fvd", this, &Pendulum::Cam_2);
	binder->BindAxis("R1", this, &Pendulum::MoveTarget);
}

void Pendulum::Cam_1(float value)
{
	if (value && cam)
	{
		float delta = 40 * value * dt;
		cam->AddComponentRotation(FQuat(0, delta, 0), eParent);
	}
}

void Pendulum::Cam_2(float value)
{
	if (value && cam)
	{
		float delta = 40 * value * dt;
		cam->AddComponentRotation(FQuat(delta, 0, 0), eParent);
	}
}

void Pendulum::MoveTarget(float value)
{
	if (value && target)
	{
		float delta = 40 * value * dt;
		target->AddComponentRotation(FQuat(0, 0, delta), eParent);
	}
}
