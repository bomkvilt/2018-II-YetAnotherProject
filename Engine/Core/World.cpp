#include "World.hpp"

#include <assert.h>

#include "Threading/ThreadPull.hpp"

#include "Actor.hpp"
#include "Object.hpp"


World::World()
	: symulationState(eUnstarted)
{
	std::string name = "scene Root";
	auto* init = new Initialiser;
	init->name = &name;
	init->world = this;

	ThreadContextSingleton::Get().PushInitialiser(init);
	sceneRoot = CreateObject<ActorComponent>();
	ThreadContextSingleton::Get().PopInitialiser();
}

World::~World()
{
	
}

void World::OnSymulationStart()
{
	symulationState = eInProgress;

	for (Object* object : objects_set) object->OnBeginPlay();
}

void World::OnSymulationStop()
{
	symulationState = eStopped;

	for (Object* object : objects_set) object->OnEndPlay();
}

void World::DoTick(float DeltaTime, ETickType type)
{
	TaskBacket backet;

	for (auto tick : tickFunctions[type])
	{
		if ( IsValid(tick->GetTarget()) ) 
		{
			backet.AddTask(new Runable([=]()
			{
				(*tick)(DeltaTime, type);
			}
			));
		}
	}
	ThreadPull::AddTaskBacket(backet);
	backet.Wait();
}

void World::RegisterTickFunction(ITickFunction& Tick)
{
	ETickType type = Tick.GetTickType();

	tickFunctions[type].emplace(&Tick);
}

void World::UnregisterTickFunction(ITickFunction& Tick)
{
	ETickType type = Tick.GetTickType();

	tickFunctions[type].erase(&Tick);
}

void World::DestroyObject(Object* object)
{
	assert(IsValid(object));

	object->OnEndPlay();
	object->OnStartDestroy();

	objects_set.erase(object);
	objects.erase(object->GetOUID());
}

bool World::IsValid(Object* base) const
{
	return base && objects.count(base->GetOUID());
}

void World::UpdateNameToUnique(std::string& name)
{
	size_t index = names[name]++;
	name += "_" + std::to_string(index);
}
