#pragma once
#ifndef MATH_HPP
#define MATH_HPP

#include "Math/Constants.hpp"
#include "Math/Color.hpp"
#include "Math/Vector2.hpp"
#include "Math/Vector.hpp"
#include "Math/Quat.hpp"
#include "Math/Transform.hpp"


float   Clamp(        float v, float min, float max);
FVector Clamp(const FVector v, float min, float max);

float   Sqrt(float x);
FVector Sqrt(const FVector& x);


#endif // !MATH_HPP
