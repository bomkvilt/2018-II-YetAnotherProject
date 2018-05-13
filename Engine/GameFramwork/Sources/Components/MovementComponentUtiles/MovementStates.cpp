#include "MovementStates.hpp"
#include "Components/MovementComponent.hpp"


FMovementStates::FMovementStates() 
	: movementMode(EMovementMode::Walk)
	, extraMode   (EMovementMode::NONE)
{
	using ConditionType = EMovementStateType;
	using ModeType      = EMovementMode;

	for (int cnd = 1; cnd < (int)ConditionType::MAX; cnd <<= 1) 
	{
		conditions[(ConditionType)cnd] = {};
	}
}

void FMovementStates::NextFrame(float DeltaTime)
{
	// calculate accelerations
	UpdateStates(DeltaTime);

	// shift states
	shiftStates();
	updateFlags.Clear();

	BModeChanged() = extraMode != EMovementMode::NONE;
	BColided()     = BColided(EMovementStateType::Last);
	extraMode      = EMovementMode::NONE;
}

void FMovementStates::UpdateStates(float DeltaTime)
{
	if (DeltaTime)
	{
		FMovementTelemetry& LastStates = conditions[EMovementStateType::Current];
		FMovementTelemetry& CurrStates = conditions[EMovementStateType::Last   ];

		if (!updateFlags.IsUpdated())
		{
			CurrStates.Acceleration = (CurrStates.Velocity - LastStates.Velocity) / DeltaTime;
			CurrStates.Epsilon      = (CurrStates.Omega    - LastStates.Omega   ) / DeltaTime;

			updateFlags.SetUpdated();
		}
	}
}

template<class _Tp>
void ShiftStatePair(_Tp& Last, _Tp& Curr)
{
	Last = Curr;
	Curr =_Tp();
}

void FMovementStates::shiftStates()
{
	FMovementTelemetry& LastStates = conditions[EMovementStateType::Current];
	FMovementTelemetry& CurrStates = conditions[EMovementStateType::Last   ];

	ShiftStatePair(LastStates, CurrStates);
}
