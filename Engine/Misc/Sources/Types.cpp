#include "Types.hpp"


void FTickFunction::operator()(float DeltaTime, ETickType type)
{
	function(DeltaTime, type);
}

Object* FTickFunction::GetTarget() const
{
	return target;
}

Object* FTickFunction::GetActor() const
{
	return actor;
}

ETickType FTickFunction::GetTickType() const
{
	return tickType;
}
