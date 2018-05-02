#include "Quat.hpp"
#include <assert.h>

#define OPERATION(_SIGN, _R) \
	X  _SIGN  _R.X; \
	Y  _SIGN  _R.Y; \
	Z  _SIGN  _R.Z; \
	W  _SIGN  _R.W;

#define OPERATION_LIST(_SIGN, _R) \
	X  _SIGN  _R.X, \
	Y  _SIGN  _R.Y, \
	Z  _SIGN  _R.Z, \
	W  _SIGN  _R.W

#define OPERATION_FLOAT(_SIGN, _R) \
	X  _SIGN  _R; \
	Y  _SIGN  _R; \
	Z  _SIGN  _R; \
	W  _SIGN  _R;

#define OPERATION_LIST_FLOAT(_SIGN, _R) \
	X  _SIGN  _R, \
	Y  _SIGN  _R, \
	Z  _SIGN  _R, \
	W  _SIGN  _R

// constants
const FQuat FQuat::Identity = FQuat(0, 0, 0, 1);


FQuat::FQuat() 
	: X(0), Y(0), Z(0), W(0)
{}

FQuat::FQuat(float X, float Y, float Z, float W)
	: X(X), Y(Y), Z(Z), W(W)
{}

FQuat::FQuat(const FVector& V, float angle)
{
	*this = fromAxisRot(V, angle);
}

FQuat::FQuat(float R, float P, float Y)
{
	*this = fromEuler(R, P, Y);
}

FQuat::FQuat(const FQuat& r)
	: X(r.X)
	, Y(r.Y)
	, Z(r.Z)
	, W(r.W)
{}

FQuat& FQuat::FQuat::operator=(const FQuat& r)
{
	OPERATION(=, r);
	return *this;
}

float& FQuat::FQuat::operator[](int i)
{
	switch (i) {
	case 0: return X;
	case 1: return Y;
	case 2: return Z;
	case 3: return W;
	}
	throw std::out_of_range("");
}

const float& FQuat::operator[](int i) const
{
	return const_cast<FQuat&>(*this)[i];
}

FQuat FQuat::operator+(const FQuat& r) const { return FQuat(OPERATION_LIST(+, r)); }
FQuat FQuat::operator-(const FQuat& r) const { return FQuat(OPERATION_LIST(-, r)); }
FQuat FQuat::operator*(const FQuat& r) const
{
	return FQuat(
		W*r.X + X*r.W + Y*r.Z - Z*r.Y,
		W*r.Y - X*r.Z + Y*r.W + Z*r.X,
		W*r.Z + X*r.Y - Y*r.X + Z*r.W,
		W*r.W - X*r.X - Y*r.Y - Z*r.Z);
}

FQuat& FQuat::operator+=(const FQuat& r) { OPERATION(+=, r); return *this; }
FQuat& FQuat::operator-=(const FQuat& r) { OPERATION(-=, r); return *this; }
FQuat& FQuat::operator*=(const FQuat& r)
{
	*this = (*this) * r;
	return *this;
}

FQuat FQuat::operator+(float r) const { return FQuat(OPERATION_LIST_FLOAT(+, r)); }
FQuat FQuat::operator-(float r) const { return FQuat(OPERATION_LIST_FLOAT(-, r)); }
FQuat FQuat::operator*(float r) const { return FQuat(OPERATION_LIST_FLOAT(*, r)); }

FQuat& FQuat::operator+=(float r) { OPERATION_FLOAT(+=, r); return *this; }
FQuat& FQuat::operator-=(float r) { OPERATION_FLOAT(-=, r); return *this; }
FQuat& FQuat::operator*=(float r) { OPERATION_FLOAT(*=, r); return *this; }

FQuat FQuat::operator-() const { return FQuat(-X, -Y, -Z, -W); }
FQuat FQuat::operator~() const { return FQuat(-X, -Y, -Z,  W); }

bool FQuat::operator==(const FQuat& r) const
{
	return X == r.X
		&& Y == r.Y
		&& Z == r.Z
		&& W == r.W;
}

bool FQuat::operator!=(const FQuat& r) const
{
	return !(*this == r);
}

float FQuat::Size() const
{
	return std::sqrtf(X*X + Y*Y + Z*Z + W*W);
}

float FQuat::SizeSq() const
{
	return X*X + Y*Y + Z*Z + W*W;
}

FQuat FQuat::GetNormal() const 
{ 
	float size = Size(); 
	return FQuat(OPERATION_LIST_FLOAT(/, size)); 

}

FQuat& FQuat::Normalise() 
{ 
	float size = Size(); 
	OPERATION_FLOAT(/=, size); 
	return *this; 
}

FQuat FQuat::Lerp(const FQuat& r, float factor)
{
	return FQuat(
		X - (r.X - X) * factor,
		Y - (r.Y - Y) * factor,
		Z - (r.Z - Z) * factor,
		W * (1 - factor) + r.W * factor);
}

FQuat FQuat::SLerp(const FQuat& r, float factor)
{
	FQuat result;
	float cosTheta = W*r.W + X*r.X + Y*r.Y + Z*r.Z;
	float theta    = std::acosf(cosTheta);
	if (std::fabs(theta) < EPSILON)
	{
		result = *this;
	}
	else
	{
		float sinTheta = std::sqrtf(1 - cosTheta * cosTheta);
		if (std::fabs(sinTheta) < EPSILON)
		{
			result.W = 0.5f*W + 0.5f*r.W;
			result.X = X - (r.X - X) * 0.5f;
			result.Y = Y - (r.Y - Y) * 0.5f;
			result.Z = Z - (r.Z - Z) * 0.5f;
		}
		else
		{
			float fA = std::sinf((1 - factor)*theta) / sinTheta;
			float fB = std::sinf(theta * factor) / sinTheta;

			result.W = W*fA + r.W*fB;
			result.X = X*fA + r.X*fB;
			result.Y = Y*fA + r.Y*fB;
			result.Z = Z*fA + r.Z*fB;
		}
	}
	return result;
}

float FQuat::AngularDistance(const FQuat r) const
{
	float innerProd = X*r.X + Y*r.Y + Z*r.Z + W*r.W;
	return RAD2DEG( std::acosf((2 * innerProd * innerProd) - 1) );
}

FVector FQuat::GetEulerAngles() const
{
	float q0 = W;
	float q1 = X;
	float q2 = Y;
	float q3 = Z;
	return RAD2DEG( FVector(
		std::atan2f(2 * (q0 * q1 + q2 * q3), 1 - 2 * (q1*q1 + q2*q2)	),
		std::asinf (2 * (q0 * q2 - q3 * q1)							),
		std::atan2f(2 * (q0 * q3 + q1 * q2), 1 - 2 * (q2*q2 + q3*q3)	)
	) );
}

FVector FQuat::Vector() const
{
	return FVector(X, Y, Z);
}

std::string FQuat::ToString() const
{
	return "X: " + std::to_string(X) + " " 
		+  "Y: " + std::to_string(Y) + " "
		+  "Z: " + std::to_string(Z) + " "
		+  "w: " + std::to_string(W);
}

FQuat FQuat::fromAxisRot(FVector axis, float angleDeg)
{
	float angleRad = DEG2RAD(angleDeg);
	float sa2 = std::sinf(angleRad / 2);
	float ca2 = std::cosf(angleRad / 2);
	return FQuat(
		axis.X * sa2,
		axis.Y * sa2,
		axis.Z * sa2,
		ca2 );
}

FQuat FQuat::fromEuler(float X, float Y, float Z)
{
	return fromAxisRot(FVector(1, 0, 0), X) 
		*  fromAxisRot(FVector(0, 1, 0), Y) 
		*  fromAxisRot(FVector(0, 0, 1), Z);
}


FVector operator*(const FQuat& q, const FVector& v)
{
	return (q * FQuat(v.X, v.Y, v.Z, 0) * ~q).Vector();
}