#pragma once

#include "HealthSystem\Health.hpp"

class Damage : public IDamage
{

public:
	Damage(int damage):damage(damage)
	{
	
	}
	virtual int GetDamage() override 
	{
		return damage;
	}
protected:
	int damage;
};


