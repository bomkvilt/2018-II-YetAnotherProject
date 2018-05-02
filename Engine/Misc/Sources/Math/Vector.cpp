#include "Vector.hpp"
#include "Vector2.hpp"

#include <assert.h>
#include <cmath>


#define OPERATION(_SIGN, _R) \
	X  _SIGN  _R.X; \
	Y  _SIGN  _R.Y; \
	Z  _SIGN  _R.Z;

#define OPERATION_LIST(_SIGN, _R) \
	X  _SIGN  _R.X, \
	Y  _SIGN  _R.Y, \
	Z  _SIGN  _R.Z

#define OPERATION_FLOAT(_SIGN, _R) \
	X  _SIGN  _R; \
	Y  _SIGN  _R; \
	Z  _SIGN  _R;

#define OPERATION_LIST_FLOAT(_SIGN, _R) \
	X  _SIGN  _R, \
	Y  _SIGN  _R, \
	Z  _SIGN  _R

#define NO_ZERO(_R) assert(_R.X); assert(_R.Y); assert(_R.Z);


// constants
const FVector FVector::ZeroVector = FVector( 0, 0, 0);
const FVector FVector::Forward    = FVector( 1, 0, 0);
const FVector FVector::Backward   = FVector(-1, 0, 0);
const FVector FVector::Upward     = FVector( 0, 1, 0);
const FVector FVector::Downward   = FVector( 0,-1, 0);
const FVector FVector::Rightward  = FVector( 0, 0, 1);
const FVector FVector::Leftward   = FVector( 0, 0,-1);


FVector::FVector()
	: X(0), Y(0), Z(0)
{}

FVector::FVector(float X, float Y, float Z)
	: X(X), Y(Y), Z(Z)
{}

FVector::FVector(const FVector& r)
	: X(r.X), Y(r.Y), Z(r.Z)
{}

FVector::FVector(const FVector2& r)
	: X(r.X), Y(r.Y), Z(0)
{}

FVector& FVector::operator=(const FVector& r)
{
	OPERATION(=, r);
	return *this;

}

FVector& FVector::operator=(const FVector2& r)
{
	X = r.X;
	Y = r.Y;
	Z = 0;
	return *this;
}

float& FVector::operator[](int i)
{
	switch (i) {
	case 0: return X;
	case 1: return Y;
	case 2: return Z;
	}
	throw std::out_of_range("");
}

const float& FVector::operator[](int i) const
{
	return const_cast<FVector&>(*this)[i];
}

// FVector - FVector

FVector FVector::operator+(const FVector& r) const {             return FVector(OPERATION_LIST(+, r)); }
FVector FVector::operator-(const FVector& r) const {             return FVector(OPERATION_LIST(-, r)); }
FVector FVector::operator*(const FVector& r) const {             return FVector(OPERATION_LIST(*, r)); }
FVector FVector::operator/(const FVector& r) const { NO_ZERO(r); return FVector(OPERATION_LIST(/, r)); }
FVector FVector::operator^(const FVector& r) const
{
	return FVector(
		Y * r.Z - r.Y * Z,
		Z * r.X - r.Z * X,
		X * r.Y - r.X * Y);
}

FVector& FVector::operator+=(const FVector& r) {             OPERATION(+=, r); return *this; }
FVector& FVector::operator-=(const FVector& r) {             OPERATION(-=, r); return *this; }
FVector& FVector::operator*=(const FVector& r) {             OPERATION(*=, r); return *this; }
FVector& FVector::operator/=(const FVector& r) { NO_ZERO(r); OPERATION(/=, r); return *this; }
FVector& FVector::operator^=(const FVector& r)
{
	X = Y * r.Z - r.Y * Z;
	Y = Z * r.X - r.Z * X;
	Z = X * r.Y - r.X * Y;
	return *this;
}

// FVector - Scalar

FVector FVector::operator+(float r) const {            return FVector(OPERATION_LIST_FLOAT(+, r)); }
FVector FVector::operator-(float r) const {            return FVector(OPERATION_LIST_FLOAT(-, r)); }
FVector FVector::operator*(float r) const {            return FVector(OPERATION_LIST_FLOAT(*, r)); }
FVector FVector::operator/(float r) const { assert(r); return FVector(OPERATION_LIST_FLOAT(/, r)); }

FVector& FVector::operator+=(float r) {             OPERATION_FLOAT(+=, r) return *this; }
FVector& FVector::operator-=(float r) {             OPERATION_FLOAT(+=, r) return *this; }
FVector& FVector::operator*=(float r) {             OPERATION_FLOAT(+=, r) return *this; }
FVector& FVector::operator/=(float r) { assert(r);  OPERATION_FLOAT(+=, r) return *this; }

// Misc

FVector FVector::operator-() const
{
	return FVector(-X, -Y, -Z);
}

bool FVector::operator==(const FVector& r) const
{
	return X == r.X
		&& Y == r.Y
		&& Z == r.Z;
}

bool FVector::operator!=(const FVector& r) const
{
	return !(*this == r);
}

float FVector::Size() const
{
	return std::sqrtf(X*X + Y*Y + Z*Z);
}

float FVector::SizeSq() const
{
	return X*X + Y*Y + Z*Z;
}

FVector FVector::GetNormal() const 
{
	float size = Size();
	return FVector(OPERATION_LIST_FLOAT(/, size));
}

FVector& FVector::Normalise()
{
	float size = Size();
	OPERATION_FLOAT(/=, size)
	return *this;
}

FVector FVector::Lerp(const FVector& r, float factor)
{
	return (*this) - (r - (*this)) * factor;
}

std::string FVector::ToString() const
{
	return "X: " + std::to_string(X) + " "
		+  "Y: " + std::to_string(Y) + " "
		+  "Z: " + std::to_string(Z);
}