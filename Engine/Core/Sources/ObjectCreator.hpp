#ifndef OBJECT_CREATOR_HPP
#define OBJECT_CREATOR_HPP
#pragma once

#include "Common.hpp"
#include "Threading/ThreadContext.hpp"

class PlayerController;


class ObjectCreator
{
public:
	template<class _T, typename... Args>
	static _T* CreateObject(std::string& name, World* world, Args&... args)
	{
		assert(world);
		auto init = Initialiser::Get();
		init->type  = EObjectType::eObject;
		init->name  = &name;
		init->world = world;

		return CreateObject<_T>(init, args...);
	}

	template<class _T, typename... Args>
	static _T* CreateAvatar(std::string& name, World* world, BasePlayerController* controller, Args&... args)
	{
		assert(world);
		auto init = Initialiser::Get();
		init->type  = EObjectType::eActor;
		init->name  = &name;
		init->world	= world;
		init->controller = controller;

		auto* point = CreateObject<_T>(init, args...);
		
		if (controller)
		{
			controller->SetTarget(point);
		}
		return point;
	}
	
	template<class _T, typename... Args>
	static _T* CreateActor(std::string& name, World* world, Args&... args)
	{
		assert(world);
		auto init = Initialiser::Get();
		init->type  = EObjectType::eActor;
		init->name  = &name;
		init->world	= world;

		return CreateObject<_T>(init, args...);
	}

	template<class _T, typename... Args>
	static _T* CreateSubcomponent(std::string& name, World* world, Actor* owner, Args&... args)
	{
		assert(world && owner);
		auto init = Initialiser::Get();
		init->type  = EObjectType::eActorComponent;
		init->name  = &name;
		init->world = world;
		init->owner = owner;

		return CreateObject<_T>(init, args...);
	}

	template<class _T, typename... Args>
	static _T* CreateSubmodule(std::string& name, World* world, Actor* owner, Args&... args)
	{
		assert(world && owner);
		auto init = Initialiser::Get();
		init->type  = EObjectType::eActorModule;
		init->name  = &name;
		init->world = world;
		init->owner = owner;

		return CreateObject<_T>(init, args...);
	}

	template<class _T, typename... Args>
	static UNIQUE(_T) CreateGameMode(Initialiser* init, Args&... args)
	{
		assert(init);
		assert(init->world);
		assert(init->controller);
		assert(init->simulation);
		SCOPE_INIT(init);

		auto point = std::make_unique<_T>(args...);
		init->controller->SetGameMode(point.get());
		return point;
	}

	template<class _T, typename... Args>
	static UNIQUE(_T) CreatePlayerController(Initialiser* init, Args&... args)
	{
		assert(init);
		assert(init->world);
		assert(init->simulation);
		SCOPE_INIT(init);
		return std::make_unique<_T>(args...);
	}

private:
	
	template<class _T, typename... Args>
	static _T* CreateObject(UNIQUE(Initialiser)& init, Args&... args)
	{
		assert(init->world);
		SCOPE_INIT(init.get());
		return init->world->CreateObject<_T>(args...);
	}
};


#endif // !OBJECT_CREATOR_HPP
