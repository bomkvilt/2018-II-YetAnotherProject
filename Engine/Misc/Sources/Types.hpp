#pragma once
#ifndef CORE_TYPES_HPP
#define CORE_TYPES_HPP

#include <assert.h>
#include <functional>
#include <array>

#include "Math.hpp"

class Object;


enum ESpaceType
{
	  eLocal
	, eWorld
	, eParent
};

enum EObjectType
{
	  eObject
	, eActor
	, eActorComponent
	, eActorModule
	, eGameMode
};

enum ETickType
{
	  ePrePhysics
	, eInPhysics
	, ePostPhysics
	, eRender
	
	, eMAX
};

enum ESimulationState
{
	  eUnstarted
	, eInProgress
	, ePaused
	, eStopped
};

enum EAttachmentRule
{
	  eKeepRelative
	, eSnapToTarget
	, eKeepWorld
};


/*******************************************************************************
*								Interfaces
*******************************************************************************/

struct ITickFunction
{
	virtual void operator()(float DeltaTime, ETickType type) = 0;

	virtual Object*   GetTarget()	const = 0;
	virtual ETickType GetTickType() const = 0;
};


/*******************************************************************************
*								Structs
*******************************************************************************/


/**
 */
struct FTickFunction : public ITickFunction
{
	ETickType tickType;
	Object*	  target;

	std::function<void(float, ETickType)> function;

public:

	virtual void operator()(float DeltaTime, ETickType type) override;

	virtual Object*		GetTarget()	  const override;
	virtual ETickType   GetTickType() const override;

public:
	
	template<class _Fx, class _T>
	void BindFunction(_T* newTarget, _Fx func)
	{
		target = newTarget;
		function = std::function<void(float, ETickType)>(std::bind(
			func, newTarget, 
			std::placeholders::_1, 
			std::placeholders::_2
			));
	}
};

/** 
 */
struct Index
{
public:
	size_t index;

public:
	Index() : index(0) {}
	size_t operator++(   ) { return ++index; }
	size_t operator++(int) { return ++index; }
	operator size_t()      { return   index; }
};


/** Object Universal ID
 */
struct OUID
{
	size_t id;

	OUID() : id(0) {}

	OUID operator++(int)
	{
		OUID curr = *this;
		return ++id, curr;
	}

	operator size_t() const
	{
		return id;
	}
};


/*******************************************************************************
*								Shape types
*******************************************************************************/

enum class EShapeType
{
	eBox
};

struct FShape
{
	FShape(EShapeType type, FVector extents) 
		: type   (type)
		, extents(extents)
	{}

	static FShape MakeBox(FVector extents)
	{
		return FShape(EShapeType::eBox, extents);
	}

public:

	EShapeType type;
	FVector extents;
};


#endif // !CORE_TYPES_HPP