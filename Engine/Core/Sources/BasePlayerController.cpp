#include "BasePlayerController.hpp"
#include "Threading/ThreadContext.hpp"

BasePlayerController::BasePlayerController()
	: world (nullptr)
	, target(nullptr)
{
	auto* init = ThreadContext::TopInitialiser();
	assert(init);
	world = init->world;
}

void BasePlayerController::OnBeginPlay()
{}

void BasePlayerController::OnEndPlay()
{}

void BasePlayerController::Tick(float DeltaTime, ETickType type)
{}
