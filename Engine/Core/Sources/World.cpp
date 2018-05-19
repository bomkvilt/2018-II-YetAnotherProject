#include "World.hpp"
#include "Actor.hpp"
#include "Threading/ThreadPool.hpp"
#include "Injection/DependencyInjectionManager.hpp"
#include "Threading/ObjectLocker.hpp"

#include <algorithm>


World::World()
	: SimulationState(eUnstarted)
{
	scene = DependencyInjectionManager::MakeScene();

	// to make a scene root it's enough to use the function
	sceneRoot = ObjectCreator::CreateObject<BaseActorComponent>(std::string("scene Root"), this);
}

World::~World()
{}

void World::OnSimulationStart()
{
	SimulationState = eInProgress;
	for (auto object : objects_set) object->OnBeginPlay();
}

void World::OnSimulationStop()
{
	SimulationState = eStopped;
	for (auto object : objects_set) object->OnEndPlay();
}

void World::DoTick(float DeltaTime, ETickType type)
{
	TaskBacket backet;

	for (auto& ticks : tickFunctions[type])
	{
		auto* actor = ticks.first;
		if (!IsValid(actor)) continue;
		
		backet.AddTask(new Runable([&, actor]()
		{
			ObjectLocker locker(actor);

			auto& ticks = tickFunctions[type][actor];
			for (auto* tick : ticks)
			{
				if (IsValid(tick->GetTarget()))
				{
					(*tick)(DeltaTime, type);
				}
			}
		} 
		));
	}
	ThreadPool::AddTaskBacket(backet);
	backet.Wait();

	if (scene && type == ETickType::eInPhysics)
	{
		scene->Update(DeltaTime);
	}
}

void World::RegisterTickFunction(ITickFunction& Tick)
{
	ETickType type = Tick.GetTickType();
	auto* target = Tick.GetTarget();
	auto* object = Tick.GetActor();
	auto* actor  = dynamic_cast<Actor*>(object);

	if (IsValid(target) && actor)
	{
		tickFunctions[type][actor].emplace(&Tick);
	}
}

void World::UnregisterTickFunction(ITickFunction& Tick)
{
	ETickType type = Tick.GetTickType();
	auto* target = Tick.GetTarget();
	auto* object = Tick.GetActor();
	auto* actor  = dynamic_cast<Actor*>(object);

	if (IsValid(target) && actor)
	{
		tickFunctions[type][actor].erase(&Tick);
	}
}

void World::DestroyObject(Object* object)
{
	assert(IsValid(object));

	object->OnEndPlay();
	object->OnStartDestroy();

	objects_set.erase(object);
	objects.erase(object->GetOUID());
}

bool World::IsValid(ObjectBase* base) const
{
	return base && objects.count(base->GetOUID());
}

/*******************************************************************************
*								Scene Iterator
*******************************************************************************/

World::SceneIterator::SceneIterator(BaseActorComponent* root)
{
	if (!root) return;
	path.push(root);
	indices.push(0);
}

BaseActorComponent* World::SceneIterator::operator->()
{
	return CurrNode();
}

BaseActorComponent& World::SceneIterator::operator*()
{
	return *CurrNode();
}

World::SceneIterator* World::SceneIterator::operator++()
{
	Move();
	return this;
}

bool World::SceneIterator::operator==(const SceneIterator& r) const
{
	size_t size_l = indices.size();
	size_t size_r = r.indices.size();
	if (size_r != size_l) return false;

	bool result = true;
	for (size_t itr = 0; itr < size_l && result; ++itr)
	{
		result &= indices._Get_container()[itr] == r.indices._Get_container()[itr];
		result &= path._Get_container()[itr] == r.path._Get_container()[itr];
	}
	return result;
}

bool World::SceneIterator::operator!=(const SceneIterator & r) const
{
	return !(*this == r);
}


BaseActorComponent* World::SceneIterator::PrevNode()
{
	return path.top();
}

BaseActorComponent* World::SceneIterator::CurrNode()
{
	return Brunch(CurrIndex());
}

BaseActorComponent* World::SceneIterator::Brunch(size_t node)
{
	return CurrLine()[node];
}

std::vector<BaseActorComponent*>& World::SceneIterator::CurrLine()
{
	return PrevNode()->GetSubcomponents();
}

size_t World::SceneIterator::CurrIndex()
{
	return indices.top();
}

size_t World::SceneIterator::MaxIndex()
{
	return CurrLine().size();
}

void World::SceneIterator::Move()
{
	auto* node = CurrNode();
	path.push(node);
	indices.push(0);

	while (path.size())
	{
		size_t index = CurrIndex();
		if (index == MaxIndex())
		{
			path.pop();
			indices.pop();
			if (path.size())
			{
				indices.top()++;
			}
		}
		else return;
	}
}
