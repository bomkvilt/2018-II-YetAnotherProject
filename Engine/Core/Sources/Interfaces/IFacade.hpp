#ifndef IFACADE_HPP
#define IFACADE_HPP
#include "Types.hpp"

class ActorComponent;

class IFacade
{
public:

	IFacade(ActorComponent* owner, FShape shape)
		: owner(owner)
		, shape(shape)
	{}
	virtual ~IFacade() = default;

public:
	
	virtual bool AttachTo(IFacade* parent) = 0;
	virtual bool Detach() = 0;

	virtual void Update() = 0;

protected:
	FShape shape;
	ActorComponent* owner;
};

#endif // !IFACADE_HPP
