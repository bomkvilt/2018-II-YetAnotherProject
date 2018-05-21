#pragma once

#include "Interfaces/IRigidBody.hpp"


enum ECollisionType
{
	  eCT_Block     = 0 // block   anything
	, eCT_Ignore    = 1 // ignore  anything
	, eCT_Overlap   = 2 // overlap anything
	, eCT_RigidBody = 3 // 
	, eCT_Trigger   = 4 // 

	, eCT_MAX
};



struct CollisionPresets
{
	CollisionPresets()
	{
		AddRule(ECollisionType::eCT_Block, [](FCollisionRules& rules)
		{
			for (int i = 0; i < ECollisionType::eCT_MAX; ++i)
			{
				rules.rules[i] = ECollisionMode::eCollide;
			}
		});
		AddRule(ECollisionType::eCT_Ignore, [](FCollisionRules& rules)
		{
			for (int i = 0; i < ECollisionType::eCT_MAX; ++i)
			{
				rules.rules[i] = ECollisionMode::eIgnore;
			}
		});
		AddRule(ECollisionType::eCT_Overlap, [](FCollisionRules& rules)
		{
			for (int i = 0; i < ECollisionType::eCT_MAX; ++i)
			{
				rules.rules[i] = ECollisionMode::eOverlap;
			}
		});
		AddRule(ECollisionType::eCT_RigidBody, [](FCollisionRules& rules)
		{
			for (int i = 0; i < ECollisionType::eCT_MAX; ++i)
			{
				rules.rules[i] = ECollisionMode::eCollide;
			}
		});
		AddRule(ECollisionType::eCT_Trigger, [](FCollisionRules& rules)
		{
			for (int i = 0; i < ECollisionType::eCT_MAX; ++i)
			{
				rules.rules[i] = ECollisionMode::eOverlap;
			}
		});
	}

	      FCollisionRules& operator[](ECollisionType type)       { return presets[type]; }
	const FCollisionRules& operator[](ECollisionType type) const { return presets[type]; }

	static CollisionPresets& Get()
	{
		return staticClass;
	}

private:

	template<typename _Clb>
	void AddRule(ECollisionType type, _Clb clb)
	{
		assert(type < FCollisionRules::maxCategoryCount);

		FCollisionRules rules;
		rules.category = type;
		clb(rules);
		presets[type] = rules;
	}

	FCollisionRules presets[ECollisionType::eCT_MAX];

	static CollisionPresets staticClass;
};