#pragma once
#ifndef CAMERA_COMPONENT_HPP
#define CAMERA_COMPONENT_HPP

#include "ActorComponent.hpp"
#include "Math/Vector2.hpp"

/** Camera component - is 
 *  
 *	. registers in it's owner controller
 *	. controller takes the camera's position
 *	. controller sets a viewer's to the position
 */
class CameraComponent : public ActorComponent
	, public ICameraComponent
{
	GENERATED_BODY(CameraComponent, ActorComponent)

public:
	CameraComponent();

	/// >> Objetc
	void OnBeginPlay() override;
	void OnEndPlay()   override;
	/// <<

public: //~~~~~~~~~~~~~~~~~~~| common

	void SetAutoregister(bool newFlag);
	bool GetAutoregister() const;

	/** Register self in player controller 
	 *  @return wether registered		*/
	bool Register(bool newState);

	/** Try to override a 
	 *	@return wether activated		*/
	bool Activate(bool newState);

	FVector GetRenderSize();
	void SetRenderSize(FVector renderSize);

protected:

	/// >> common
	bool bActive;		// is being used by owner's controller
	bool bRegistered;	// is registered
	bool bAutoRegister;	// regisers automaticly
	/// <<
	FVector RenderSize;
};





#endif // !CAMERA_HPP
