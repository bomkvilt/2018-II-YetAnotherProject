#pragma once
#ifndef IFACADE_HPP
#define IFACADE_HPP

class BaseActorComponent;

class IFacade
{
public:

	IFacade(BaseActorComponent* owner)
		: owner(owner)
	{}
	virtual ~IFacade() = default;

public:
	
	virtual bool AttachTo(IFacade* parent) = 0;
	virtual bool Detach() = 0;

	virtual void Update() = 0;

protected:
	
	BaseActorComponent* owner;
};

#endif // !IFACADE_HPP
