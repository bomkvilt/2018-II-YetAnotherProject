#include "World.hpp"
#include "Actor.hpp"
#include "Threading/ThreadPool.hpp"
#include "Injection/DependencyInjectionManager.hpp"

#include <algorithm>


World::World()
	: SimulationState(eUnstarted)
{
	scene = DependencyInjectionManager::MakeScene();

	// to make a scene root it's enough to use the function
	sceneRoot = ObjectCreator::CreateObject<ActorComponent>(std::string("scene Root"), this);
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
	//TaskBacket backet;

	for (auto* tick : tickFunctions[type])
	{
		if ( IsValid(tick->GetTarget()) ) 
		{
		//	backet.AddTask(new Runable([=]()
		//	{
				(*tick)(DeltaTime, type);
		//	}
		//	));
		}
	}
	//ThreadPool::AddTaskBacket(backet);
	//backet.Wait();

	if (scene && type == ETickType::eInPhysics)
	{
		scene->Update(DeltaTime);
	}
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

/*******************************************************************************
*								Scene Iterator
*******************************************************************************/

World::SceneIterator::SceneIterator(ActorComponent* root)
{
	if (!root) return;
	path.push(root);
	indices.push(0);
}

ActorComponent* World::SceneIterator::operator->()
{ 
	return CurrNode(); 
}

ActorComponent& World::SceneIterator::operator*()
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
	size_t size_l =   indices.size();
	size_t size_r = r.indices.size();
	if (size_r != size_l) return false;
	
	bool result = true;
	for (size_t itr = 0; itr < size_l && result; ++itr)
	{
		result &= indices._Get_container()[itr] == r.indices._Get_container()[itr];
		result &= path   ._Get_container()[itr] == r.path   ._Get_container()[itr];
	}
	return result;
}

bool World::SceneIterator::operator!=(const SceneIterator & r) const
{
	return !(*this == r);
}


ActorComponent* World::SceneIterator::PrevNode()
{
	return path.top();
}

ActorComponent* World::SceneIterator::CurrNode()
{
	return Brunch(CurrIndex());
}

ActorComponent* World::SceneIterator::Brunch(size_t node)
{
	return CurrLine()[node];
}

std::vector<ActorComponent*>& World::SceneIterator::CurrLine()
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
	path   .push(node);
	indices.push(0);

	while (path.size())
	{
		size_t index = CurrIndex();
		if (index == MaxIndex())
		{
			path   .pop();
			indices.pop();
			if (path.size())
			{
				indices.top()++;
			}
		}
		else return;
	}
}
