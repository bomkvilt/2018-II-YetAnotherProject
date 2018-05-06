#pragma once
#ifndef QUAT_HPP
#define QUAT_HPP
#pragma once

#include "Vector.hpp"
#include "Constants.hpp"
#include "Reflection/Archived.hpp"


struct FQuat : public Archived
{
	GENERATED_ARCHIVATION_BODY(FQuat, Archived);
		ARCHIVE_MEMBER("", X);
		ARCHIVE_MEMBER("", Y);
		ARCHIVE_MEMBER("", Z);
		ARCHIVE_MEMBER("", W);
		ARCHIVATION_END()
public:
	float X;
	float Y;
	float Z;
	float W;

public:

	FQuat();
	FQuat(float X, float Y, float Z, float W);
	FQuat(const FVector& V, float angle);
	FQuat(float R, float P, float Y);
	FQuat(const FQuat& r);

	FQuat& operator=(const FQuat& r);

	      float& operator[](int idx);
	const float& operator[](int idx) const;

public: //~~~~~~~~~~~~~~| quat - quat

	FQuat operator+(const FQuat& r) const;
	FQuat operator-(const FQuat& r) const;
	FQuat operator*(const FQuat& r) const;

	// ?=

	FQuat& operator+=(const FQuat& r);
	FQuat& operator-=(const FQuat& r);
	FQuat& operator*=(const FQuat& r);

public: //~~~~~~~~~~~~~~| quat - scalar

	FQuat operator+(float r) const;
	FQuat operator-(float r) const;
	FQuat operator*(float r) const;

	// ?=

	FQuat& operator+=(float r);
	FQuat& operator-=(float r);
	FQuat& operator*=(float r);

public: //~~~~~~~~~~~~~~| unary

	FQuat operator-() const;
	FQuat operator~() const;

public: //~~~~~~~~~~~~~~| equation

	bool operator==(const FQuat& r) const;
	bool operator!=(const FQuat& r) const;

public: //~~~~~~~~~~~~~~| size

	float Size  () const;
	float SizeSq() const;

	FQuat  GetNormal() const;
	FQuat& Normalise();

public: //~~~~~~~~~~~~~~| misc

	FQuat Lerp (const FQuat& r, float factor);
	FQuat SLerp(const FQuat& r, float factor);

	float AngularDistance(const FQuat r) const;

	FVector GetEulerAngles() const;
	FVector Vector() const;

	std::string ToString() const;

public: //~~~~~~~~~~~~~~| static

	static FQuat fromAxisRot(FVector axis, float angleDeg);
	static FQuat fromEuler  (float X, float Y, float Z);

public: //~~~~~~~~~~~~~~| constants

	static const FQuat Identity;
};


FVector operator*(const FQuat& q, const FVector& v);


#endif // !QUAT_HPP
