#pragma once
#ifndef IFACADE_HPP
#define IFACADE_HPP

class ActorComponent;

class IFacade
{
public:

	IFacade(ActorComponent* owner)
		: owner(owner)
	{}
	virtual ~IFacade() = default;

public:
	
	virtual bool AttachTo(IFacade* parent) = 0;
	virtual bool Detach() = 0;

	virtual void Update() = 0;

protected:
	
	ActorComponent* owner;
};

#endif // !IFACADE_HPP
