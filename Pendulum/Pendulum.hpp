#pragma once

#include "Core/Avatar.hpp"
#include "Components/BoxColision.hpp"
#include "Components/CameraComponent.hpp"


class Pendulum : public Avatar
{
	using Super = Avatar;

public:

	Pendulum()
	{
		base = CreateSubComponent<BoxColision>("box");
		box1 = CreateSubComponent<BoxColision>("box");
		box2 = CreateSubComponent<BoxColision>("box");
		cam1 = CreateSubComponent<CameraComponent>("Camera");

		box2->AttachTo(box1);
		cam1->AttachTo(box2);
		cam1->SetAutoregister(true);

		box2->SetExtends({
			btScalar(1), 
			btScalar(1), 
			btScalar(1)
			});
		auto* node1 = box2->GetFacade().mesh->getChild(0);
		if (auto* shape = dynamic_cast<osg::ShapeDrawable*>(node1))
		{
			shape->setColor(osg::Vec4(
				0, 1, 0, 0
			));
		}

		base->SetExtends({
			btScalar(2), 
			btScalar(2), 
			btScalar(0.01)
			});
		auto* node2 = base->GetFacade().mesh->getChild(0);
		if (auto* shape = dynamic_cast<osg::ShapeDrawable*>(node2))
		{
			shape->setColor(osg::Vec4(
				0, 0, 0, 0
			));
		}

		btScalar pi4 = osg::PI/4;
		//box1->AddComponentLocation({ 1 , 0 , 0}, eWorld);
		//box1->AddComponentRotation({pi4, 0, 0}, eWorld);
		box2->AddComponentLocation({ 0 , 0, 2}, eLocal);
		cam1->AddComponentLocation({ 0 , 0, 3}, eLocal);
	}

	virtual void Tick(float DeltaTime, ETickType type)
	{
		float angle = osg::PI / 10 * DeltaTime;
		float speed =			3  * DeltaTime;

		float a1 = angle * R1;
		float a2 = angle * R2;
		float a3 = angle * R3;

		float s1 = speed * bLMK;
		float s2 = speed * bRMK;

		Rotate(box1, a1, a2, 0 );
		Rotate(box2, 0 , 0 , a3);
		Move  (box2, 0 , 0 , s1);
		Move  (box2, 0 , 0 ,-s2);
	}

	virtual void SetupInput(EventBinder* binder)
	{
		Super::SetupInput(binder);

		binder->BindAxis("R1", this, &Pendulum::RoateR1);
		binder->BindAxis("R2", this, &Pendulum::RoateR2);
		binder->BindAxis("R3", this, &Pendulum::RoateR3);
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
	BoxColision* base = nullptr;
	CameraComponent* cam1 = nullptr;

	float R1 = 0;
	float R2 = 0;
	float R3 = 0;

	bool bLMK = 0;
	bool bRMK = 0;
};