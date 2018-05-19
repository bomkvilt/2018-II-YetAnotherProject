#pragma once
#ifndef TRANSFORM_HPP
#define TRANSFORM_HPP
#pragma once

#include "Quat.hpp"
#include "Vector.hpp"


class FTransform : public Archived
{
	GENERATED_ARCHIVATION_BODY(FTransform, Archived);
		ARCHIVE_MEMBER("", Location);
		ARCHIVE_MEMBER("", Rotation);
		ARCHIVATION_END()
public:

	FVector Location; // positions of a center
	FQuat   Rotation; // ratation around the center

public:

	FTransform();
	FTransform(const FQuat& q, const FVector& v);
	FTransform(const FQuat& q);
	FTransform(const FVector& v);
	FTransform(const FTransform& r);

	FTransform& operator=(const FTransform& r);

public: //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~| Rotatins and Translations

	/** Apply the transform to the vector 
	* @param v - vector in a transform's parent coorditates
	* @return	- rotated and translated vector					*/
	FVector operator()(const FVector& v) const;
	FVector operator* (const FVector& v) const;

	/** Apply the transform to the quaternion
	* @param q - quaternion in a transform's parent coorditates
	* @return	- rotated quaternion							*/
	FQuat operator()(const FQuat& q) const;
	FQuat operator* (const FQuat& q) const;

public: //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~| 

	FTransform operator*(const FTransform& r) const;

	// ?=

	FTransform& operator*=(const FTransform& r);
	FTransform& operator+=(const FTransform& r);
	FTransform& operator+=(const FQuat& r);
	FTransform& operator+=(const FVector& r);

public: //~~~~~~~~~~~~~~| unary

	FTransform operator~() const;

public: //~~~~~~~~~~~~~~| misc

	std::string ToString() const;

public: //~~~~~~~~~~~~~~| constants

	static const FTransform Identity;
};


FTransform operator*(const FQuat& q, const FTransform& t);


#endif // TRANSFORM_HPP
