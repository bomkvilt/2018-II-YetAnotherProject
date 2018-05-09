#pragma once

#include "Avatar.hpp"
#include "BoxColision.hpp"
#include "CameraComponent.hpp"
#include "SpriteComponent.hpp"
#include "Control/EventBinder.hpp"
#include "PID.hpp"

#include <iostream>


class Pendulum : public Avatar
{
	GENERATED_BODY(Pendulum, Avatar);
public:

	Pendulum()
		: pid(4, 0, 3)
	{
		base = CreateSubcomponent<BoxColision>("Base");
		base->SetExtents(FVector(0.6f, 0.6f, 0.6f));
		
		pendulum = CreateSubcomponent<BoxColision>("Pendulum");
		pendulum->AddComponentLocation(FVector(2, 0, 0), eParent);
		pendulum->AddComponentRotation(FQuat(0, 0, 30), eParent);
		pendulum->SetExtents(FVector(2, 0.2f, 0.2f));

		sprite = CreateSubcomponent<SpriteComponent>("sprite", "box.png");
		sprite->AddComponentLocation(FVector(0, 0, 0.1f));
		sprite->SetExtents(FVector(2, 0.2f, 0.2f));
		sprite->AttachTo(pendulum);
		
		target = CreateSubcomponent<BoxColision>("Target");
		target->AddComponentLocation(FVector(5, 0 , 0), eParent);
		target->AddComponentRotation(FQuat(0 ,0, 90), eParent);
		target->SetExtents(FVector(0.5f, 0.1f, 0.1f));

		cam = CreateSubcomponent<CameraComponent>("Camera");
		cam->AddComponentLocation(FVector(-30, 0, 0), eParent);
		cam->AddComponentRotation(FQuat( 0, 0 ,90), eParent);
		cam->SetAutoregister(true);
	}

	virtual void Tick(float DeltaTime, ETickType type)
	{
		dt = DeltaTime;

		if (!DeltaTime) return;

		auto tr = target  ->GetComponentRotation();
		auto cr = pendulum->GetComponentRotation();
		//std::cout << cr.ToString()<<std::endl;
		auto delta = ~cr * tr;
		
		const float control = pid.GetValue(delta.Z, dt);
		const float inertia = 600;
		const float M0      = 100;
		
		const float M  = M0 * control;
		const float dw = RAD2DEG(M / inertia * dt);
		w += dw;
		
		pendulum->AddComponentRotation(FQuat(0, 0, w * dt), eParent);
	}

	virtual void SetupInput(EventBinder* binder)
	{
		Super::SetupInput(binder);
		binder->BindAxis("Bvd", this, &Pendulum::Cam_1);
		binder->BindAxis("Fvd", this, &Pendulum::Cam_2);
		binder->BindAxis("R1", this, &Pendulum::MoveTarget);
	}

protected:

	void Cam_1(float value)
	{
		if (value && cam)
		{
			float delta = 40 * value * dt;
			cam->AddComponentRotation(FQuat(0, 0, delta), eParent);
		}
	}

	void Cam_2(float value)
	{
		if (value && cam)
		{
			float delta = 40 * value * dt;
			cam->AddComponentRotation(FQuat(delta, 0, 0), eParent);
		}
	}

	void MoveTarget(float value)
	{
		if (value && target)
		{
			float delta = 40 * value * dt;
			target->AddComponentRotation(FQuat(0, 0, delta), eParent);
		}
	}

protected:


protected:

	BoxColision* base       = nullptr;
	BoxColision* target     = nullptr;
	BoxColision* pendulum   = nullptr;
	SpriteComponent* sprite = nullptr;
	CameraComponent* cam    = nullptr;

	float dt = 0;
	float w  = 0;

	PID pid;
};

// convare