#ifndef CORE_TYPES_HPP
#define CORE_TYPES_HPP
#pragma once

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

	virtual Object*   GetTarget()   const = 0;
	virtual Object*   GetActor()    const = 0;
	virtual ETickType GetTickType() const = 0;
};


/*******************************************************************************
*								Structs
*******************************************************************************/


/**
 */
struct FTickFunction : public ITickFunction
{
	ETickType tickType = ETickType::eMAX;
	Object*	  target   = nullptr;
	Object*   actor    = nullptr;

	std::function<void(float, ETickType)> function;

public:

	virtual void operator()(float DeltaTime, ETickType type) override;

	virtual Object*	  GetTarget()	const override;
	virtual Object*   GetActor()    const override;
	virtual ETickType GetTickType() const override;

public:
	
	template<class _Fx, class _T, class _A>
	void BindFunction(_T* newTarget, _A* newActor, _Fx func)
	{
		target = newTarget;
		actor  = newActor;

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


/*******************************************************************************
*								constraint types
*******************************************************************************/

enum EAxisType
{
	  eX   = 0
	, eY   = 1
	, eZ   = 2
	, AXT_MAX = 3
};

enum EAngleType
{
	  ePitch = 1
	, eRoll  = 0
	, eYaw   = 2
	, AGT_MAX   = 3
};


struct FConstraintType
{
	struct FAxisConstaint
	{
		float min = 0; // minimal value 
		float max = 0; // maximal value
		bool  bAcive = false; // is the constraint active
	};

public: //~~~~~~~~~~~~~~| constraints

	std::array<FAxisConstaint, EAngleType::AGT_MAX> rotation;
	std::array<FAxisConstaint, EAxisType ::AXT_MAX> movement;

public: //~~~~~~~~~~~~~~| constructors

	FConstraintType() = default;

	// 3 - sphere
	static FConstraintType MakeSphere()
	{
		FConstraintType res;
		res.RotationConstaraint(ePitch, true);
		res.RotationConstaraint(eYaw  , true);
		res.RotationConstaraint(eRoll , true);
		return res;
	}

	// 3 - movement
	static FConstraintType MakeMovement()
	{
		FConstraintType res;
		res.MovementConstarint(eX, true);
		res.MovementConstarint(eY, true);
		res.MovementConstarint(eZ, true);
		return res;
	}

	// 4 - cylinder
	static FConstraintType MakeCylinder(EAxisType axis)
	{
		FConstraintType res;
		EAngleType angle = (EAngleType)axis;
		res.MovementConstarint (axis , true);
		res.RotationConstaraint(angle, true);
		return res;
	}

	// 5 - rotation
	static FConstraintType MakeRotation(EAngleType angle, float min = 0, float max = 0)
	{
		FConstraintType res;
		res.SetRotation(angle, min, max);
		return res;
	}

	// 5 - axis
	static FConstraintType MakeAxis(EAxisType axis, float min = 0, float max = 0)
	{
		FConstraintType res;
		res.SetMovement(axis, min, max);
		return res;
	}

	// 6 - seal
	static FConstraintType MakeSeal()
	{
		FConstraintType res;
		res.MovementConstarint(eX, true);
		res.MovementConstarint(eY, true);
		res.MovementConstarint(eZ, true);

		res.RotationConstaraint(ePitch, true);
		res.RotationConstaraint(eYaw  , true);
		res.RotationConstaraint(eRoll , true);
		return res;
	}

public:

	void SetRotation(EAngleType angle, float min, float max)
	{
		rotation[angle].bAcive = true;
		rotation[angle].max = max;
		rotation[angle].min = min;
	}
	void RotationConstaraint(EAngleType angle, bool state)
	{
		rotation[angle].bAcive = state;
	}

	void SetMovement(EAxisType axis, float min, float max)
	{
		movement[axis].bAcive = true;
		movement[axis].max = max;
		movement[axis].min = min;
	}
	void MovementConstarint(EAxisType axis, bool state)
	{
		movement[axis].bAcive = state;
	}

public:

	FVector GetAngularLower() const
	{
		FVector values;
		for (int i = 0; i < EAngleType::AGT_MAX; ++i)
		{
			values[i] = rotation[i].min;
		}
		return values;
	}

	FVector GetAngularUpper() const
	{
		FVector values;
		for (int i = 0; i < EAngleType::AGT_MAX; ++i)
		{
			values[i] = rotation[i].max;
		}
		return values;
	}

	FVector GetLinearLower() const
	{
		FVector values;
		for (int i = 0; i < EAxisType::AXT_MAX; ++i)
		{
			values[i] = movement[i].min;
		}
		return values;
	}

	FVector GetLinearUpper() const
	{
		FVector values;
		for (int i = 0; i < EAxisType::AXT_MAX; ++i)
		{
			values[i] = movement[i].max;
		}
		return values;
	}
};


/*******************************************************************************
*								Collision types
*******************************************************************************/

struct FHit
{
	class Actor* actor = nullptr;
	class Actor* other = nullptr;
	class BaseActorComponent* component      = nullptr;
	class BaseActorComponent* otherComponent = nullptr;

	FVector Location;
	FVector HitNormal;
};


#endif // !CORE_TYPES_HPP