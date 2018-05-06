#include "BoxColision.hpp"


BoxColision::BoxColision()
	: extents(1, 1, 1)
{}

void BoxColision::SetExtents(FVector newExtents)
{
	extents = newExtents;
}
