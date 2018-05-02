#include "Transform.hpp"

// constants
const FTransform FTransform::Identity = FTransform();




FTransform::FTransform()
	: Location (FVector::ZeroVector)
	, Rotation (FQuat::Identity)
{}

FTransform::FTransform(const FQuat& q, const FVector& v)
	: Location (v)
	, Rotation (q)
{}

FTransform::FTransform(const FQuat& q)
	: Location (FVector::ZeroVector)
	, Rotation (q)
{}

FTransform::FTransform(const FVector& v)
	: Location (v)
	, Rotation (FQuat::Identity)
{}

FTransform::FTransform(const FTransform& r)
	: Location (r.Location)
	, Rotation (r.Rotation)
{}

FTransform& FTransform::operator=(const FTransform& r)
{
	Location = r.Location;
	Rotation = r.Rotation;
	return *this;
}

FVector FTransform::operator()(const FVector& v) const
{
	return Rotation * v + Location;
}

FVector FTransform::operator*(const FVector& v) const
{
	return (*this)(v);
}

FQuat FTransform::operator()(const FQuat& q) const
{
	return Rotation * q;
}

FQuat FTransform::operator*(const FQuat& q) const
{
	return (*this)(q);
}

FTransform FTransform::operator*(const FTransform& r) const
{
	FTransform tmp = *this;
	return (tmp *= r);
}

FTransform& FTransform::operator*=(const FTransform& r)
{
	Location = (*this) * r.Location;
	Rotation *= r.Rotation;
	return *this;
}

FTransform& FTransform::operator+=(const FTransform& r)
{
	Rotation = r.Rotation * Rotation;
	Location = r.Location + Location;
	return *this;
}

FTransform& FTransform::operator+=(const FQuat& r)
{
	Rotation = r * Rotation;
	return *this;
}

FTransform& FTransform::operator+=(const FVector& r)
{
	Location += r;
	return *this;
}

FTransform FTransform::operator~() const
{
	FQuat inv = ~Rotation;
	return FTransform(inv, inv * -Location);
}

std::string FTransform::ToString() const
{
	return "R:{" + Rotation.ToString() + "}"
		+  "LL{" + Location.ToString() + "}";
}




FTransform operator*(const FQuat& q, const FTransform& t)
{
	return FTransform(q * t.Rotation, q * t.Location);
}