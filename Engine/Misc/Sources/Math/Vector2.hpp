#pragma once
#ifndef VECTOR2_HPP
#define VECTOR2_HPP

#include <string>
#include "Vector.hpp"


struct FVector2 : public Archived
{
	GENERATED_ARCHIVATION_BODY(FVector2, Archived);
		ARCHIVE_MEMBER("", X);
		ARCHIVE_MEMBER("", Y);
		ARCHIVATION_END()
public:
	float X;
	float Y;

public:

	FVector2();
	FVector2(float X, float Y);
	FVector2(const FVector & r);
	FVector2(const FVector2& r);

	FVector2& operator=(const FVector & r);
	FVector2& operator=(const FVector2& r);

	      float& operator[](int i);
	const float& operator[](int i) const;

public: //~~~~~~~~~~~~~~| vector - vector

	FVector2 operator+(const FVector2& r) const;
	FVector2 operator-(const FVector2& r) const;
	FVector2 operator*(const FVector2& r) const;
	FVector2 operator/(const FVector2& r) const;
	FVector  operator^(const FVector2& r) const;

	// ?=

	FVector2& operator-=(const FVector2& r);
	FVector2& operator+=(const FVector2& r);
	FVector2& operator*=(const FVector2& r);
	FVector2& operator/=(const FVector2& r);

public: //~~~~~~~~~~~~~~| vector - scalar

	FVector2 operator+(float r) const;
	FVector2 operator-(float r) const;
	FVector2 operator*(float r) const;
	FVector2 operator/(float r) const;

	// ?=

	FVector2& operator+=(float r);
	FVector2& operator-=(float r);
	FVector2& operator*=(float r);
	FVector2& operator/=(float r);

public: //~~~~~~~~~~~~~~| unary

	FVector2 operator-() const;

public: //~~~~~~~~~~~~~~| equation

	bool operator==(const FVector2& r) const;
	bool operator!=(const FVector2& r) const;

public: //~~~~~~~~~~~~~~| size

	float Size  () const;
	float SizeSq() const;

	FVector2  GetNormal() const;
	FVector2& Normalise();

public: //~~~~~~~~~~~~~~| misc

	FVector2 Lerp(const FVector2& r, float factor);

	std::string ToString() const;

public: //~~~~~~~~~~~~~~| constants

	static const FVector2 ZeroVector;

	static const FVector2 Forward;
	static const FVector2 Backward;
	
	static const FVector2 Upward;
	static const FVector2 Downward;
};


#endif //!VECTOR2_HPP