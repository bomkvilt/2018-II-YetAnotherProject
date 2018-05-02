#include "Color.hpp"

#define OPERATION(_SIGN, _R) \
	R  _SIGN  _R.R; \
	G  _SIGN  _R.G; \
	B  _SIGN  _R.B; \
	A  _SIGN  _R.A;

#define OPERATION_LIST(_SIGN, _R) \
	R  _SIGN  _R.R, \
	G  _SIGN  _R.G, \
	B  _SIGN  _R.B, \
	A  _SIGN  _R.A

#define OPERATION_FLOAT(_SIGN, _R) \
	R  _SIGN  _R; \
	G  _SIGN  _R; \
	B  _SIGN  _R; \
	A  _SIGN  _R;

#define OPERATION_LIST_FLOAT(_SIGN, _R) \
	R  _SIGN  _R, \
	G  _SIGN  _R, \
	B  _SIGN  _R, \
	A  _SIGN  _R

// constants
const FColor FColor::Red   = FColor(1, 0, 0);
const FColor FColor::Green = FColor(0, 1, 0);
const FColor FColor::Blue  = FColor(0, 0, 1);


FColor::FColor(float R, float G, float B, float A)
	: R(R), G(G), B(B), A(A)
{}

FColor::FColor(const FColor & r)
	: R(r.R), G(r.G), B(r.B), A(r.A)
{}

FColor& FColor::operator=(const FColor& r)
{
	OPERATION(=, r)
	return *this;
}

float& FColor::operator[](int i) 
{
	switch (i) {
	case 0: return R;
	case 1: return G;
	case 2: return B;
	case 3: return A;
	}
	throw std::out_of_range("");
}

const float& FColor::operator[](int i) const
{
	return const_cast<FColor&>(*this)[i];
}

// FColor

FColor  FColor::operator+(const FColor& r) const { return FColor(OPERATION_LIST(+, r)); }
FColor  FColor::operator-(const FColor& r) const { return FColor(OPERATION_LIST(-, r)); }

FColor& FColor::operator-=(FColor r) { OPERATION(-=, r) return *this; }
FColor& FColor::operator+=(FColor r) { OPERATION(+=, r) return *this; }

// Scalar

FColor  FColor::operator+(float r) const {            return FColor(OPERATION_LIST_FLOAT(+, r)); }
FColor  FColor::operator-(float r) const {            return FColor(OPERATION_LIST_FLOAT(-, r)); }
FColor  FColor::operator*(float r) const {            return FColor(OPERATION_LIST_FLOAT(*, r)); }
FColor  FColor::operator/(float r) const { assert(r); return FColor(OPERATION_LIST_FLOAT(/, r)); }

FColor& FColor::operator+=(float r) {            OPERATION_FLOAT(+=, r) return *this; }
FColor& FColor::operator-=(float r) {            OPERATION_FLOAT(-=, r) return *this; }
FColor& FColor::operator*=(float r) {            OPERATION_FLOAT(*=, r) return *this; }
FColor& FColor::operator/=(float r) { assert(r); OPERATION_FLOAT(/=, r) return *this; }

// Comapre

bool FColor::operator==(const FColor& r) const
{
	return R == r.R
		&& G == r.G
		&& B == r.B
		&& A == r.A;
}

bool FColor::operator!=(const FColor& r) const
{
	return !(*this == r);
}

// Misc

FColor FColor::Lerp(const FColor & r, float factor)
{
	return (*this) - (r - (*this)) * factor;
}

std::string FColor::ToString() const
{
	return "R: " + std::to_string(R) + " "
		+  "G: " + std::to_string(G) + " "
		+  "B: " + std::to_string(B) + " "
		+  "A: " + std::to_string(A);
}
