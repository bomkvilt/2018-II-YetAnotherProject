//---------------------------------------------
#pragma once
#include "ActorModule.hpp"
#include <functional>
#include <map>


struct IDamage
{
 virtual ~IDamage() {}

public:
 virtual int GetDamage() = 0;
};


struct IDamageable
{
 virtual ~IDamageable() {}

public:
 virtual void ApplyDamage(IDamage* damage) = 0;
};


class HealthModule : public ActorModule
 , public IDamageable
{
 GENERATED_BODY(HealthModule, ActorModule)

public:
 virtual void ApplyDamage(IDamage* damage)
 {
  if (damage)
  {
   health -= damage->GetDamage();

		TryToCallCallbacks();
  }
 }

 /**
  * Add a callback will be called 
  * when the health is lower than @currentHealth
  */
 template<typename _Clb>
 void AddCallback(_Clb clb, int currentHealth)
 {
  callbacks.emplace(currentHealth, clb);
 }

 //void RemoveCallback(...)

protected:
 void TryToCallCallbacks()
 {
  auto itr = callbacks.find(health);
  auto end = callbacks.end();

  for (; itr != end; ++itr)
  {
   itr->second(health);
  }
 }

protected:
 int health;

 std::multimap<int, std::function<void(int)>> callbacks;
};