#include "B2Convertion.hpp"

b2Vec2& operator<<(b2Vec2& l, const FVector& r)
{
	l.x = r.X;
	l.y = r.Y;
	return l;
}

FVector& operator<<(FVector& l, const b2Vec2& r)
{
	l.X = r.x;
	l.Y = r.y;
	return l;
}

b2Transform& operator<<(b2Transform& l, const FTransform& r)
{
	l.Set(b2Vec2() << r.Location, r.Rotation.Z);
	return l;
}

FTransform& operator<<(FTransform& l, const b2Transform& r)
{
	l.Location << r.p;
	l.Rotation = FQuat(0, 0, RAD2DEG(r.q.GetAngle()));
	return l;
}
