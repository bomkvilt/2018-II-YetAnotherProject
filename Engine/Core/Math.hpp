#ifndef MATH_HPP
#define MATH_HPP

#include "vmath.h"



class Transform
{
public:

	Vector3f origin; // center position
	Quatf  rotation; // rotation around the center

public:

	Transform() = default;

	explicit Transform(const Quatf& q, const Vector3f& v)
		: origin	(v)
		, rotation	(q)
	{}

	explicit Transform(const Quatf& q)
		: origin	(Vector3f(0,0,0))
		, rotation	(q)
	{}

	explicit Transform(const Vector3f& v)
		: origin	(v)
		, rotation	(Quatf::getIdentity())
	{}

	Transform(const Transform& r)
		: origin	(r.origin)
		, rotation	(r.rotation)
	{}

	Transform& operator=(const Transform& r)
	{
		origin   = r.origin;
		rotation = r.rotation;
		return *this;
	}

	static Transform getIdentity()
	{
		return Transform(Quatf::getIdentity());
	}

public: //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~| Rotatins and Translations

	/** Apply the transform to the vector 
	 * @param v - vector in a transform's parent coorditates
	 * @return	- rotated and translated vector					*/
	Vector3f operator()(const Vector3f& v) const
	{
		return rotation * v + origin;
	}

	Vector3f operator*(const Vector3f& v) const
	{
		return (*this)(v);
	}

	/** Apply the transform to the quaternion
	 * @param q - quaternion in a transform's parent coorditates
	 * @return	- rotated quaternion							*/
	Quatf operator()(const Quatf& q) const
	{
		return rotation * q;
	}

	Quatf operator*(const Quatf& q) const
	{
		return (*this)(q);
	}

public: //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~| 

	Transform& operator*=(const Transform& r)
	{
		origin = *this * r.origin;
		rotation *= r.rotation;
		return *this;
	}

	Transform operator*(const Transform& r) const
	{
		Transform tmp = *this;
		return (tmp *= r);
	}

	Transform& operator+=(const Transform& r)
	{
		rotation = r.rotation * rotation;
		origin   = r.origin   + origin  ;
		return *this;
	}

	Transform& operator+=(const Quatf& r)
	{
		rotation = r * rotation;
		return *this;
	}

	Transform& operator+=(const Vector3f& r)
	{
		origin += r;
		return *this;
	}

	Transform operator~() const
	{
		return inverse();
	}

	Transform inverse() const
	{
		Quatf inv = ~rotation;
		return Transform(inv, inv * -origin);
	}

public: //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~| Get && Set

	Quatf	 getRotation()	const	{ return rotation; }
	Vector3f getOrigin()	const 	{ return origin; }

	void SetRotation(Quatf	  newRotation)	{ rotation = newRotation; }
	void SetOrigin  (Vector3f newOrigin	 )	{ origin   = newOrigin;	  }
};


inline Transform operator*(const Quatf& q, const Transform& t)
{
	return Transform(q * t.rotation, q * t.origin);
}


#endif // !MATH_HPP
