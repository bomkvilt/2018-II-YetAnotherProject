#pragma once
#ifndef GOSHA_HPP
#define GOSHA_HPP

#include "Avatar.hpp"
#include "Components/BoxColision.hpp"
#include "Components/SpriteComponent.hpp"
#include "Components/CameraComponent.hpp"
#include "Components/MovementComponent.hpp"
#include "PlayerController.hpp"

#include "Blocks/Block.hpp"


class Gosha : public Avatar
{
	GENERATED_BODY(Gosha, Avatar);

public:
	Gosha();

	virtual void OnBeginPlay() override;

	void SetupInput(EventBinder* binder);

protected:
	void Move(float value);
	void Jump(EKeyAction);
	void Keep(EKeyAction);

protected:
	BoxColision* body;
	SpriteComponent* sprite;
	CameraComponent* camera;

protected:
	Block* contactedBlock;

	Block* attachedBlock;

protected:
	void AttachBlock(Block* block);
	void DetachBlock();
};


#endif // !GOSHA_HPP
