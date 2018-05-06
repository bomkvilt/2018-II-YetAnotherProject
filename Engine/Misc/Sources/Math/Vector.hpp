#pragma once
#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <string>
#include "Reflection/Archived.hpp"

struct FVector2;


struct FVector : public Archived
{
	GENERATED_ARCHIVATION_BODY(FVector, Archived);
		ARCHIVE_MEMBER("", X);
		ARCHIVE_MEMBER("", Y);
		ARCHIVE_MEMBER("", Z);
		ARCHIVATION_END()
public:
	float X;
	float Y;
	float Z;

public:

	FVector();
	FVector(float X, float Y, float Z);
	FVector(const FVector & r);
	FVector(const FVector2& r);

	FVector& operator=(const FVector & r);
	FVector& operator=(const FVector2& r);

	      float& operator[](int i);
	const float& operator[](int i) const;

public: //~~~~~~~~~~~~~~| vector - vector

	FVector operator+(const FVector& r) const;
	FVector operator-(const FVector& r) const;
	FVector operator*(const FVector& r) const;
	FVector operator/(const FVector& r) const;
	FVector operator^(const FVector& r) const;

	// ?=

	FVector& operator-=(const FVector& r);
	FVector& operator+=(const FVector& r);
	FVector& operator*=(const FVector& r);
	FVector& operator/=(const FVector& r);
	FVector& operator^=(const FVector& r);

public: //~~~~~~~~~~~~~~| vector - scalar

	FVector operator+(float r) const;
	FVector operator-(float r) const;
	FVector operator*(float r) const;
	FVector operator/(float r) const;

	// ?=

	FVector& operator+=(float r);
	FVector& operator-=(float r);
	FVector& operator*=(float r);
	FVector& operator/=(float r);

public: //~~~~~~~~~~~~~~| unary

	FVector operator-() const;

public: //~~~~~~~~~~~~~~| equation

	bool operator==(const FVector& r) const;
	bool operator!=(const FVector& r) const;

public: //~~~~~~~~~~~~~~| size

	float Size  () const;
	float SizeSq() const;

	FVector  GetNormal() const;
	FVector& Normalise();

public: //~~~~~~~~~~~~~~| misc

	FVector Lerp(const FVector& r, float factor);

	std::string ToString() const;

public: //~~~~~~~~~~~~~~| constants

	static const FVector ZeroVector;

	static const FVector Forward;
	static const FVector Backward;
	
	static const FVector Upward;
	static const FVector Downward;
	
	static const FVector Rightward;
	static const FVector Leftward;
};

#endif //!VECTOR_HPP