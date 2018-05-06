#ifndef OBJECT_CREATOR_HPP
#define OBJECT_CREATOR_HPP
#pragma once

#include "Common.hpp"
#include "Threading/ThreadContext.hpp"

class PlayerController;


class ObjectCreator
{
public:
	template<class _T>
	static _T* CreateObject(std::string& name, World* world)
	{
		assert(world);
		auto init = Initialiser::Get();
		init->type  = EObjectType::eObject;
		init->name  = &name;
		init->world = world;

		return CreateObject<_T>(init);
	}

	template<class _T>
	static _T* CreateAvatar(std::string& name, World* world, BasePlayerController* controller)
	{
		assert(world);
		auto init = Initialiser::Get();
		init->type  = EObjectType::eActor;
		init->name  = &name;
		init->world	= world;
		init->controller = controller;

		auto* point = CreateObject<_T>(init);
		
		if (controller)
		{
			controller->SetTarget(point);
		}
		return point;
	}
	
	template<class _T>
		static _T* CreateActor(std::string& name, World* world)
	{
		assert(world);
		auto init = Initialiser::Get();
		init->type  = EObjectType::eActor;
		init->name  = &name;
		init->world	= world;

		return CreateObject<_T>(init);
	}

	template<class _T>
	static _T* CreateSubcomponent(std::string& name, World* world, Actor* owner)
	{
		assert(world && owner);
		auto init = Initialiser::Get();
		init->type  = EObjectType::eActorComponent;
		init->name  = &name;
		init->world = world;
		init->owner = owner;

		return CreateObject<_T>(init);
	}

	template<class _T>
	static _T* CreateSubmodule(std::string& name, World* world, Actor* owner)
	{
		assert(world && owner);
		auto init = Initialiser::Get();
		init->type  = EObjectType::eActorModule;
		init->name  = &name;
		init->world = world;
		init->owner = owner;

		return CreateObject<_T>(init);
	}

	template<class _T>
	static UNIQUE(_T) CreateGameMode(Initialiser* init)
	{
		assert(init);
		assert(init->world);
		assert(init->controller);
		assert(init->simulation);
		SCOPE_INIT(init);

		auto point = UNIQUE(_T)(new _T);
		init->controller->SetGameMode(point.get());
		return point;
	}

	template<class _T>
	static UNIQUE(_T) CreatePlayerController(Initialiser* init)
	{
		assert(init);
		assert(init->world);
		assert(init->simulation);
		SCOPE_INIT(init);
		return UNIQUE(_T)(new _T);
	}

private:
	
	template<class _T>
	static _T* CreateObject(UNIQUE(Initialiser)& init)
	{
		assert(init->world);
		SCOPE_INIT(init.get());
		return init->world->CreateObject<_T>();
	}
};


#endif // !OBJECT_CREATOR_HPP
