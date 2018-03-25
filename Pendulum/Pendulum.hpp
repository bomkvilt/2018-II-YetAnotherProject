#pragma once

#include "Core/Avatar.hpp"
#include "Components/BoxColision.hpp"
#include "Components/CameraComponent.hpp"



#define SETUP_BOX(BOX, E1, E2, E3, C1, C2, C3, C4)					\
	{																\
		BOX->SetExtends({E1, E2, E3});								\
		auto* node = BOX->GetFacade().mesh->getChild(0);			\
		if (auto* shape = dynamic_cast<osg::ShapeDrawable*>(node))	\
			shape->setColor({C1, C2, C3, C4});						\
	}


class Pendulum : public Avatar
{
	using Super = Avatar;

public:

	Pendulum()
	{
		base = CreateSubComponent<BoxColision>("Base");
		box1 = CreateSubComponent<BoxColision>("box");
		box2 = CreateSubComponent<BoxColision>("box");
		axis = CreateSubComponent<BoxColision>("Axis");
		cam1 = CreateSubComponent<CameraComponent>("Camera");

		box2->AttachTo(box1);
		axis->AttachTo(box1);
		cam1->AttachTo(box2);
		cam1->SetAutoregister(true);

		SETUP_BOX(box2, .1f , .1f , .1f , 0, 1, 0, 0);
		SETUP_BOX(base,  2  ,  2  , .01f, 0, 0, 0, 0);
		SETUP_BOX(axis, .01f, .01f, 10, 1, 0, 0, 0);
		
		float pi2 = 180/2;
		float pi4 = 180/4;
		box1->AddComponentLocation({ 1 , 0, 0}, eLocal);
		box1->AddComponentRotation({pi4, 0, 0}, eLocal);
		box2->AddComponentLocation({ 0 , 0, 2}, eLocal);
		cam1->AddComponentLocation({ 0 , 0, 3}, eLocal);
	}

	virtual void Tick(float DeltaTime, ETickType type)
	{
		float angle = 15 * DeltaTime;
		float speed = 3  * DeltaTime;

		float a1 = angle * R1;
		float a2 = angle * R2;
		float a3 = angle * R3;

		float s1 = speed * bLMK;
		float s2 = speed * bRMK;
		float s3 = speed * move;

		if(a1 || a2)	Rotate(box1, a1, a2, 0 );
		if(a3)			Rotate(box2, 0 , 0 , a3);
		if(s3)			Move  (box1, s3, 0 , 0 );
		if(s1)			Move  (box2, 0 , 0 , s1);
		if(s2)			Move  (box2, 0 , 0 ,-s2);
	}

	virtual void SetupInput(EventBinder* binder)
	{
		Super::SetupInput(binder);

		binder->BindAxis("R1",   this, &Pendulum::RoateR1);
		binder->BindAxis("R2",   this, &Pendulum::RoateR2);
		binder->BindAxis("R3",   this, &Pendulum::RoateR3);
		binder->BindAxis("Move", this, &Pendulum::MoveBase);
		binder->BindAction("Push",	  BA_Pressed,  this, &Pendulum::Push);
		binder->BindAction("Click_L", BA_Pressed,  this, &Pendulum::LMK);
		binder->BindAction("Click_L", BA_Released, this, &Pendulum::LMK);
		binder->BindAction("Click_R", BA_Pressed,  this, &Pendulum::RMK);
		binder->BindAction("Click_R", BA_Released, this, &Pendulum::RMK);
	}

protected:

	void RoateR1(float value) { R1 = value; }
	void RoateR2(float value) { R2 = value; }
	void RoateR3(float value) { R3 = value; }
	void MoveBase(float value) { move = value; }

	void LMK(EKeyAction type) { bLMK = (type == BA_Pressed); }
	void RMK(EKeyAction type) { bRMK = (type == BA_Pressed); }

	void Push(EKeyAction)
	{
		box2->AddComponentLocation({0,0,2}, eLocal);
	}

protected:

	void Rotate(BoxColision* target, float ax, float ay, float az)
	{
		target->AddComponentRotation({ax, ay, az}, eLocal);
	}

	void Move(BoxColision* target, float ax, float ay, float az)
	{
		target->AddComponentLocation({ax, ay, az}, eLocal);
	}

protected:

	BoxColision* box1 = nullptr;
	BoxColision* box2 = nullptr;
	BoxColision* axis = nullptr;
	BoxColision* base = nullptr;
	CameraComponent* cam1 = nullptr;

	float R1 = 0;
	float R2 = 0;
	float R3 = 0;
	float move = 0;

	bool bLMK = 0;
	bool bRMK = 0;
};