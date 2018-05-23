#include "Math.hpp"

float Clamp(float v, float min, float max)
{
	return v > max ? max
		: v < min ? min
		: v;
}

FVector Clamp(const FVector v, float min, float max)
{
	return FVector(
		Clamp(v.X, min, max),
		Clamp(v.Y, min, max),
		Clamp(v.Z, min, max)
	);
}

float Sqrt(float x)
{
	return std::sqrt(x);
}

FVector Sqrt(const FVector& x)
{
	return FVector(
		Sqrt(x.X),
		Sqrt(x.Y),
		Sqrt(x.Z)
		);
}
