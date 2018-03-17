#pragma once

#include "World.hpp"
#include "ObjectBase.hpp"

class Object : public ObjectBase
{
	using Super = ObjectBase;

public:

	Object();

public: //~~~~~~~~~~~~~~| LifeCycle

	virtual void OnConstructed();
	virtual void OnBeginPlay();
	virtual void OnEndPlay();
	virtual void OnStartDestroy();

public: //~~~~~~~~~~~~~~| Common

	bool IsValid(Object* object) const;

public: //~~~~~~~~~~~~~~| Tick

	virtual void Tick(float DeltaTime, ETickType type);

	void RegisterTick  (ITickFunction& tick);
	void UnregisterTick(ITickFunction& tick);

protected:

	/// >> tIck
	TickFunction PrimatyTick;
	/// << 

public: //~~~~~~~~~~~~~~| Templates

	template<class _T>
	_T* MakeObject(std::string name)
	{
		assert(world);

		auto* init = new Initialiser;
		init->name = &name;
		init->world = world;

		ThreadContextSingleton::Get().PushInitialiser(init);

		auto* point = world->CreateObject<_T>();

		ThreadContextSingleton::Get().PopInitialiser();

		return point;
	}
};