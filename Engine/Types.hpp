#ifndef CORE_TYPES_HPP
#define CORE_TYPES_HPP

#include <assert.h>
#include <functional>

#include "Core/Math.hpp"

class Object;


enum ESpaceType
{
	eLocal,
	eWorld
};

enum ETickType
{
	ePrePhysics,
	eInPhysics,
	ePostPhysics,
	eRender,

	eMAX
};

enum ESymulationState
{
	eUnstarted,
	eInProgress,
	ePaused,
	eStopped
};

enum EAttachmentRule
{
	eKeepRelative,
	eSnapToTarget,
	eKeepWorld
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
struct TickFunction : public ITickFunction
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
	void BindFuctoin(_Fx func, _T* newTarget)
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
	size_t index;

public:

	Index() : index(0) {}

	size_t operator++() {
		return ++index;
	}

	size_t operator++(int) {
		return ++index;
	}

	operator size_t() {
		return index;
	}
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


#endif // !CORE_TYPES_HPP