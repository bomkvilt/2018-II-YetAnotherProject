#include "Types.hpp"


void TickFunction::operator()(float DeltaTime, ETickType type)
{
	function(DeltaTime, type);
}

Object* TickFunction::GetTarget() const
{
	return target;
}

ETickType TickFunction::GetTickType() const
{
	return tickType;
}
