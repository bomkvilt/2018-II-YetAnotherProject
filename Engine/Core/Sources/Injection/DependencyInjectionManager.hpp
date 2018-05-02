#ifndef MODULE_MANAGER_HPP
#define MODULE_MANAGER_HPP

#include "IFrontendFabric.hpp"
#include "IPhysicsFabric.hpp"


class DependencyInjectionManager
{
public:
	static void SetFrontendFabric(UNIQUE(IFrontendFabric) fabric);
	static UNIQUE(IFacade) MakeFacade(ActorComponent* owner);
	static UNIQUE(IViewer) MakeViewer(PlayerController* controller, SHARED(FEngineConfig) config);

	static void SetPhysicsConfig(UNIQUE(IPhysicsFabric) conf);
	static UNIQUE(IPhysicsScene) MakeScene();
	static UNIQUE(IConstraint  ) MakeConstraint(ActorComponent* owner);
	static UNIQUE(IRigidBody   ) MakeBody(FShape shape, ActorComponent* owner, float mass = 0, FVector inertia = FVector::ZeroVector);
private:

	static UNIQUE(IFrontendFabric) frontend;
	static UNIQUE(IPhysicsFabric ) physics;
};


#endif // !MODULE_MANAGER_HPP
