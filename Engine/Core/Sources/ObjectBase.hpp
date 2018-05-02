#pragma once

#include <string>

#include "Misc.hpp"
#include "Types.hpp"

#include "ObjectCreator.hpp"
#include "Reflection/Archived.hpp"

class World;



class ObjectBase : public Archived
{
	GENERATED_BODY(ObjectBase, Archived);

	friend class ObjectCreator;
public:

	ObjectBase();
	virtual ~ObjectBase();

public: //~~~~~~~~~~~~~~| LifeCycle

	virtual void OnConstructed();
	virtual void OnBeginPlay();
	virtual void OnEndPlay();
	virtual void OnStartDestroy();

public: //~~~~~~~~~~~~~~| Access

	/// ouid

	const OUID GetOUID() const	{ return ouid; }
	void SetOUID(OUID newOUID)	{ ouid = newOUID; }

	/// name

	const std::string& GetName() const		 { return name; }
	void SetName(const std::string& newName) { name = newName; }

	/// world

	const World* GetWorld() const	{ return world; }
		  World* GetWorld()			{ return world; }
	void SetWorld(World* newWorld)	{ world = newWorld; }

	/// type

	EObjectType GetType() const		{ return type; }

protected: //~~~~~~~~~~~| Object meta

	/// >> common
	OUID		ouid;
	World*		world;
	EObjectType type;
	std::string name;
	/// << 

public: //~~~~~~~~~~~~~~| Creation functions

	template<class _T>
	_T* CreateObject(std::string name)
	{
		return ObjectCreator::CreateObject<_T>(name, world);
	}

	template<class _T>
	_T* CreateActor(std::string name,  bool AttachToController = false)
	{
		return ObjectCreator::CreateActor<_T>(name, world, AttachToController ? playerController : nullptr);
	}
};