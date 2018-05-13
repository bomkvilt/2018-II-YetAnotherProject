#pragma once
#ifndef MOVEMENT_STATES_HPP
#define MOVEMENT_STATES_HPP

#include <unordered_map>
#include "Actor.hpp"


enum class EMovementMode 
{
	  NONE

	, Run	         ///
	, Walk			 ///
	, Lying			 ///
	, Sprint		 ///
	, Jumping		 ///
	, Crouching		 ///

	, Swimming_slow  ///
	, Swimming_fast	 ///
	
	, falling        ///
	, flying		 ///
	
	, MAX
};

enum class EMovementStateType {
	  Current	= 1 << 0
	, Last		= 1 << 1
	, MAX
};

enum class EMovementImpactType
{		  
	  Force        /// input = force / torque
	, Impulce      /// imput = (angular) impulce
	, Acceleration /// input = (angular) accelration 
};


struct FMovementMode
{
public:	// << Normal (target) kinematic values
	FVector Velocity     = FVector::ZeroVector;
	FVector Omega        = FVector::ZeroVector;
	FVector Acceleration = FVector::ZeroVector;
	FVector Epsilon      = FVector::ZeroVector;

public: // << extra fields
	FVector Force          = FVector::ZeroVector; /// EMovementImpactType == Force
	FVector Torque         = FVector::ZeroVector; /// EMovementImpactType == Force
	FVector Impulce        = FVector::ZeroVector; /// EMovementImpactType == Impulce
	FVector AngularImpulce = FVector::ZeroVector; /// EMovementImpactType == Impulce

public:	// << Control values
	EMovementImpactType ImpactType = EMovementImpactType::Acceleration;

	float CriticalAngle = 10; /// angle were a stabilisation torque tajkes a maximal value
};


struct FMovementTelemetry
{
public: // << kinematic telemetry
	FVector Velocity        = FVector::ZeroVector;
	FVector Acceleration    = FVector::ZeroVector;
	FVector Omega           = FVector::ZeroVector;
	FVector Epsilon         = FVector::ZeroVector;
	FTransform Transform    = FVector::ZeroVector;

public: // << control
	FVector LinearInput     = FVector::ZeroVector;
	FVector AngularInput    = FVector::ZeroVector;
	FVector Force           = FVector::ZeroVector;
	FVector Torque          = FVector::ZeroVector;
	FVector Impulce         = FVector::ZeroVector;
	FVector AngularImpoulce = FVector::ZeroVector;

public: // <<
	bool bColided     = false;
	bool bModeChanged = false;
};


struct FMovementStatesFlags
{
	int bUseStabilisation : 1;

public:
	FMovementStatesFlags()
		: bUseStabilisation(1)
	{}
};


struct FMovementStates
{
public:
	struct FUpdateFlags
	{	
	public:
		FUpdateFlags() : changed(0) {}

		void Clear()	         { changed = 0;    }
		explicit operator bool() { return changed; }

		void SetUpdated	 (int flag = 1) { changed |=  flag;	     }
		void UnsetUpdated(int flag = 1) { changed &= ~flag;	     }
		bool IsUpdated	 (int flag = 1) { return changed & flag; }

	protected:
		int changed;
	};

public:
	FMovementStates();

protected:	//~~~~~~~~~~~~~~~~~~~| Conditions
	/// telemetry
	mutable std::unordered_map< 
		EMovementStateType, 
		FMovementTelemetry
		> conditions;

public:	
	/// calculate final states and flush them into last slots
	void NextFrame(float DeltaTime);
	/// calculate final states for the state type
	void UpdateStates(float DeltaTime);
	/// move current states into a last slot
	void shiftStates();

public:
	/// << control inut
	      FVector& LinearInput   (EMovementStateType Time = EMovementStateType::Current)       { return conditions[Time].LinearInput;     }
	const FVector& LinearInput   (EMovementStateType Time = EMovementStateType::Current) const { return conditions[Time].LinearInput;     }
	      FVector& AngularInput  (EMovementStateType Time = EMovementStateType::Current)       { return conditions[Time].AngularInput;    }
	const FVector& AngularInput  (EMovementStateType Time = EMovementStateType::Current) const { return conditions[Time].AngularInput;    }
	      FVector& Force         (EMovementStateType Time = EMovementStateType::Current)       { return conditions[Time].Force;           }
	const FVector& Force         (EMovementStateType Time = EMovementStateType::Current) const { return conditions[Time].Force;           }
	      FVector& Torque        (EMovementStateType Time = EMovementStateType::Current)       { return conditions[Time].Torque;          }
	const FVector& Torque        (EMovementStateType Time = EMovementStateType::Current) const { return conditions[Time].Torque;          }
	      FVector& Impulce       (EMovementStateType Time = EMovementStateType::Current)       { return conditions[Time].Impulce;         }
	const FVector& Impulce       (EMovementStateType Time = EMovementStateType::Current) const { return conditions[Time].Impulce;         }
	      FVector& AngularImpulce(EMovementStateType Time = EMovementStateType::Current)       { return conditions[Time].AngularImpoulce; }
	const FVector& AngularImpulce(EMovementStateType Time = EMovementStateType::Current) const { return conditions[Time].AngularImpoulce; }

	/// << 
	      bool& BColided    (EMovementStateType Time = EMovementStateType::Current)       { return conditions[Time].bColided;     }
	const bool& BColided    (EMovementStateType Time = EMovementStateType::Current) const { return conditions[Time].bColided;     }
	      bool& BModeChanged(EMovementStateType Time = EMovementStateType::Current)       { return conditions[Time].bModeChanged; }
	const bool& BModeChanged(EMovementStateType Time = EMovementStateType::Current) const { return conditions[Time].bModeChanged; }

protected:	//~~~~~~~~~~~~~~~~| Modes 
	/// work modes
	mutable std::unordered_map<
		EMovementMode, 
		FMovementMode
		> modes;

public:
	/// active mode
	EMovementMode movementMode;
	/// temporal modifer
	EMovementMode extraMode;

	EMovementMode CurrentModeType() const 
	{
		return extraMode == EMovementMode::NONE ? movementMode : extraMode; 
	}

	/// get the mode
	FMovementMode& GetMode(EMovementMode mode)       { return modes[mode]; }
	FMovementMode  GetMode(EMovementMode mode) const { return modes[mode]; }
	
	/// get a current mode
	FMovementMode& CurrentMode()       { return GetMode(CurrentModeType()); }
	FMovementMode  CurrentMode() const { return GetMode(CurrentModeType()); }

public:	//~~~~~~~~~~~~~~~~~~~| Flags 
	/// flags shows wethre a state were update
	FUpdateFlags updateFlags;
	/// main flags
	FMovementStatesFlags flags;

public:	//~~~~~~~~~~~~~~~~~~~| Easy Acces
		//***| current values |

	FVector& Velocity()                { return conditions[EMovementStateType::Current].Velocity;     }
	FVector& Acceleration()            { return conditions[EMovementStateType::Current].Acceleration; }
	FVector& Omega()                   { return conditions[EMovementStateType::Current].Omega;        }
	FVector& Epsilon()                 { return conditions[EMovementStateType::Current].Epsilon;      }
	FTransform& Transform()            { return conditions[EMovementStateType::Current].Transform;    }

	FVector Velocity()           const { return conditions[EMovementStateType::Current].Velocity;     }
	FVector Acceleration()       const { return conditions[EMovementStateType::Current].Acceleration; }
	FVector Omega()              const { return conditions[EMovementStateType::Current].Omega;        }
	FVector Epsilon()            const { return conditions[EMovementStateType::Current].Epsilon;      }
	FTransform Transform()       const { return conditions[EMovementStateType::Current].Transform;    }

public: //***| last values |

	FVector LastVelocity()       const { return conditions[EMovementStateType::Last].Velocity;        }
	FVector LastAcceleration()   const { return conditions[EMovementStateType::Last].Acceleration;    }
	FVector LastOmega()          const { return conditions[EMovementStateType::Last].Omega;           }
	FVector LastEpsilon()        const { return conditions[EMovementStateType::Last].Epsilon;         }
	FTransform LastTransform()   const { return conditions[EMovementStateType::Last].Transform;       }

public: //***| normal values |

	EMovementImpactType& ImpactType()      { return CurrentMode().ImpactType;     }
	FVector& NormalSpeed()                 { return CurrentMode().Velocity;       }
	FVector& NormalOmega()                 { return CurrentMode().Omega;          }
	FVector& NormalAcceleration()          { return CurrentMode().Acceleration;   }
	FVector& NormalEpsilon()               { return CurrentMode().Epsilon;        }
	FVector& NormalForce()                 { return CurrentMode().Force;          }
	FVector& NormalTorque()                { return CurrentMode().Torque;         }
	FVector& NormalImpulce()               { return CurrentMode().Impulce;        }
	FVector& NormalAngularImpulce()        { return CurrentMode().AngularImpulce; }
	float&   CriticalAngle()               { return CurrentMode().CriticalAngle;  }

	EMovementImpactType ImpactType() const { return CurrentMode().ImpactType;     }
	FVector NormalSpeed()            const { return CurrentMode().Velocity;       }
	FVector NormalOmega()            const { return CurrentMode().Omega;          }
	FVector NormalAcceleration()     const { return CurrentMode().Acceleration;   }
	FVector NormalEpsilon()          const { return CurrentMode().Epsilon;        }
	FVector NormalForce()            const { return CurrentMode().Force;          }
	FVector NormalTorque()           const { return CurrentMode().Torque;         }
	FVector NormalImpulce()          const { return CurrentMode().Impulce;        }
	FVector NormalAngularImpulce()   const { return CurrentMode().AngularImpulce; }
	float   CriticalAngle()          const { return CurrentMode().CriticalAngle;  }
};

#endif // !MOVEMENT_STATES_HPP