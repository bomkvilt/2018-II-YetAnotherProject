#ifndef COLOR_HPP
#define COLOR_HPP
#pragma once

#include <assert.h>
#include <string>
#include "Reflection/Archived.hpp"


struct FColor : public Archived
{
	GENERATED_ARCHIVATION_BODY(FColor, Archived)
		ARCHIVE_MEMBER("", R);
		ARCHIVE_MEMBER("", G);
		ARCHIVE_MEMBER("", B);
		ARCHIVE_MEMBER("", A);
		ARCHIVATION_END();
public:

	float R;
	float G;
	float B;
	float A;

public:

	FColor(float R = 0, float G = 0, float B = 0, float A = 0);
	FColor(const FColor& r);
	
	FColor& operator=(const FColor& r);

	      float& operator[](int i);
	const float& operator[](int i) const;

public: //~~~~~~~~~~~~~~| color - color

	FColor operator+(const FColor& r) const;
	FColor operator-(const FColor& r) const;

	FColor& operator-=(FColor r);
	FColor& operator+=(FColor r);

public: //~~~~~~~~~~~~~~| color - scalar

	FColor operator+(float r) const;
	FColor operator-(float r) const;
	FColor operator*(float r) const;
	FColor operator/(float r) const;

	// ?=

	FColor& operator+=(float r);
	FColor& operator-=(float r);
	FColor& operator*=(float r);
	FColor& operator/=(float r);

public: //~~~~~~~~~~~~~~| equation

	bool operator==(const FColor& r) const;
	bool operator!=(const FColor& r) const;

public: //~~~~~~~~~~~~~~| misc

	FColor Lerp(const FColor& r, float factor);

	std::string ToString() const;

public: //~~~~~~~~~~~~~~| constants

	static const FColor Red;
	static const FColor Green;
	static const FColor Blue;
};

#endif //!COLOR_HPP