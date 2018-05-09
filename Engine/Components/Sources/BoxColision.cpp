#include "BoxColision.hpp"


BoxColision::BoxColision()
{
	extents = FVector(1,1,1);
}

void BoxColision::SetExtents(const FVector& newExtents)
{
	extents = newExtents;
}
