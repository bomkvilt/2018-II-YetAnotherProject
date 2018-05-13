#pragma once
#ifndef PENDULUM_HPP
#define PENDULUM_HPP

#include <iostream>

#include "Avatar.hpp"
#include "Components/BoxColision.hpp"
#include "Components/JointComponent.hpp"
#include "Components/CameraComponent.hpp"
#include "Control/EventBinder.hpp"
#include "PID.hpp"

#include "Delegation/Delegate.hpp"

class Pendulum : public Avatar
{
	GENERATED_BODY(Pendulum, Avatar);

public:
	Pendulum();

	virtual void OnBeginPlay() override;
	virtual void Tick(float DeltaTime, ETickType type);
	virtual void SetupInput(EventBinder* binder);

protected:

	void Cam_1(float value);
	void Cam_2(float value);
	void MoveTarget(float value);

protected:

	BoxColision* base;
	BoxColision* floor;
	BoxColision* target;
	BoxColision* jumper;
	BoxColision* pendulum;
	JointComponent*	joint;
	CameraComponent* cam;

	float dt = 0;
	float w  = 0;

	PID pid;
};

#endif // !PENDULUM_HPP