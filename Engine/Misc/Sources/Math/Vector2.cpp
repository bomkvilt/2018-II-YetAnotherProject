#include "Vector2.hpp"

#include <assert.h>
#include <cmath>


#define OPERATION(_SIGN, _R) \
	X  _SIGN  _R.X; \
	Y  _SIGN  _R.Y;

#define OPERATION_LIST(_SIGN, _R) \
	X  _SIGN  _R.X, \
	Y  _SIGN  _R.Y

#define OPERATION_FLOAT(_SIGN, _R) \
	X  _SIGN  _R; \
	Y  _SIGN  _R;

#define OPERATION_LIST_FLOAT(_SIGN, _R) \
	X  _SIGN  _R, \
	Y  _SIGN  _R

#define NO_ZERO(_R) assert(_R.X); assert(_R.Y);


// constants
const FVector2 FVector2::ZeroVector = FVector2( 0, 0);
const FVector2 FVector2::Forward    = FVector2( 1, 0);
const FVector2 FVector2::Backward   = FVector2(-1, 0);
const FVector2 FVector2::Upward     = FVector2( 0, 1);
const FVector2 FVector2::Downward   = FVector2( 0,-1);


FVector2::FVector2()
	: X(0), Y(0)
{}

FVector2::FVector2(float X, float Y)
	: X(X), Y(Y)
{}

FVector2::FVector2(const FVector& r)
	: X(r.X), Y(r.Y)
{}

FVector2::FVector2(const FVector2& r)
	: X(r.X), Y(r.Y)
{}

FVector2& FVector2::operator=(const FVector & r)
{
	X = r.X;
	Y = r.Y;
	return *this;
}

FVector2& FVector2::operator=(const FVector2& r)
{
	OPERATION(=, r);
	return *this;

}

float& FVector2::operator[](int i)
{
	switch (i) {
	case 0: return X;
	case 1: return Y;
	}
	throw std::out_of_range("");
}

const float& FVector2::operator[](int i) const
{
	return const_cast<FVector2&>(*this)[i];
}

// FVector2 - FVector2

FVector2 FVector2::operator+(const FVector2& r) const {             return FVector2(OPERATION_LIST(+, r)); }
FVector2 FVector2::operator-(const FVector2& r) const {             return FVector2(OPERATION_LIST(-, r)); }
FVector2 FVector2::operator*(const FVector2& r) const {             return FVector2(OPERATION_LIST(*, r)); }
FVector2 FVector2::operator/(const FVector2& r) const { NO_ZERO(r); return FVector2(OPERATION_LIST(/, r)); }
FVector  FVector2::operator^(const FVector2& r) const
{
	return FVector(0, 0, X * r.Y - r.X * Y);
}

FVector2& FVector2::operator+=(const FVector2& r) {             OPERATION(+=, r); return *this; }
FVector2& FVector2::operator-=(const FVector2& r) {             OPERATION(-=, r); return *this; }
FVector2& FVector2::operator*=(const FVector2& r) {             OPERATION(*=, r); return *this; }
FVector2& FVector2::operator/=(const FVector2& r) { NO_ZERO(r); OPERATION(/=, r); return *this; }

// FVector2 - Scalar

FVector2 FVector2::operator+(float r) const {            return FVector2(OPERATION_LIST_FLOAT(+, r)); }
FVector2 FVector2::operator-(float r) const {            return FVector2(OPERATION_LIST_FLOAT(-, r)); }
FVector2 FVector2::operator*(float r) const {            return FVector2(OPERATION_LIST_FLOAT(*, r)); }
FVector2 FVector2::operator/(float r) const { assert(r); return FVector2(OPERATION_LIST_FLOAT(/, r)); }

FVector2& FVector2::operator+=(float r) {             OPERATION_FLOAT(+=, r) return *this; }
FVector2& FVector2::operator-=(float r) {             OPERATION_FLOAT(+=, r) return *this; }
FVector2& FVector2::operator*=(float r) {             OPERATION_FLOAT(+=, r) return *this; }
FVector2& FVector2::operator/=(float r) { assert(r);  OPERATION_FLOAT(+=, r) return *this; }

// Misc

FVector2 FVector2::operator-() const
{
	return FVector2(-X, -Y);
}

bool FVector2::operator==(const FVector2& r) const
{
	return X == r.X
		&& Y == r.Y;
}

bool FVector2::operator!=(const FVector2& r) const
{
	return !(*this == r);
}

float FVector2::Size() const
{
	return std::sqrtf(X*X + Y*Y);
}

float FVector2::SizeSq() const
{
	return X*X + Y*Y;
}

FVector2 FVector2::GetNormal() const 
{
	float size = Size();
	return FVector2(OPERATION_LIST_FLOAT(/, size));
}

FVector2& FVector2::Normalise()
{
	float size = Size();
	OPERATION_FLOAT(/=, size)
	return *this;
}

FVector2 FVector2::Lerp(const FVector2& r, float factor)
{
	return (*this) - (r - (*this)) * factor;
}

std::string FVector2::ToString() const
{
	return "X: " + std::to_string(X) + " "
		+  "Y: " + std::to_string(Y);;
}