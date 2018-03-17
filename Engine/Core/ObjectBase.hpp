#pragma once

#include <string>

#include "../Misc.hpp"
#include "../Types.hpp"

#include "Threading/ThreadContext.hpp"

class World;



class ObjectBase
{
public:

	ObjectBase();
	virtual ~ObjectBase();

public:

	/// ouid

	const OUID GetOUID() const	{ return ouid; }
		  OUID GetOUID()		{ return ouid; }
	void SetOUID(OUID newOUID)	{ ouid = newOUID; }

	/// name

	const std::string& GetName() const		 { return name; }
		  std::string& GetName()			 { return name; }
	void SetName(const std::string& newName) { name = newName; }

	/// world

	const World* GetWorld() const	{ return world; }
		  World* GetWorld()			{ return world; }
	void SetWorld(World* newWorld)	{ world = newWorld; }

protected:

	/// >> common
	OUID		ouid;
	World*		world;
	std::string name;
	/// << 
};