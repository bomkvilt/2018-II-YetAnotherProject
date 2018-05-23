#pragma once
#ifndef B2CONVERSION_HPP
#define B2CONVERSION_HPP

#include "Box2D/Box2D.h"
#include "Common.hpp"


b2Vec2&  operator<<(b2Vec2 & l, const FVector& r);
FVector& operator<<(FVector& l, const b2Vec2 & r);

b2Transform& operator<<(b2Transform& l, const FTransform&  r);
FTransform&  operator<<(FTransform & l, const b2Transform& r);

#endif // !B2CONVERSION_HPP
