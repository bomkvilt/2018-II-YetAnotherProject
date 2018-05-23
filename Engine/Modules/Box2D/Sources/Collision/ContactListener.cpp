#include "ContactListener.hpp"
#include "B2Convertion.hpp"

#include <iostream>

void ContactListener::BeginContact(b2Contact* contact)
{
	if (contact)
	if (contact->GetFixtureA() && contact->GetFixtureA()->GetBody())
	if (contact->GetFixtureB() && contact->GetFixtureB()->GetBody())
	{
		auto* A = contact->GetFixtureA()->GetBody();
		auto* B = contact->GetFixtureB()->GetBody();
		
		b2WorldManifold mfA;
		contact->GetWorldManifold(&mfA);

		b2WorldManifold mfB = mfA;
		mfB.normal *= 1;

		SetContactStatrted(A, B, mfA);
		SetContactStatrted(B, A, mfB);
	}
}

void ContactListener::EndContact(b2Contact* contact)
{
	if (contact)
	if (contact->GetFixtureA() && contact->GetFixtureA()->GetBody())
	if (contact->GetFixtureB() && contact->GetFixtureB()->GetBody())
	{
		auto* A = contact->GetFixtureA()->GetBody();
		auto* B = contact->GetFixtureB()->GetBody();

		SetContactFinished(A, B);
		SetContactFinished(B, A);
	}
}

void ContactListener::Flush()
{
	newConacts      .clear();
	FinishedContacts.clear();
}

void ContactListener::SetContactFinished(b2Body* A, b2Body* B)
{
	if (!FinishedContacts.count(A))
	{
		FinishedContacts[A] = {B};
	}
	else
	{
		FinishedContacts[A].emplace_back(B);
	}
}

void ContactListener::SetContactStatrted(b2Body* A, b2Body* B, b2WorldManifold mf)
{
	FContactData hit;
	hit.body  = A;
	hit.other = B;
	hit.Location  << mf.points[0];
	hit.HitNormal << mf.normal;

	if (!newConacts.count(A))
	{
		newConacts[A] = { {B, hit} };
	}
	else
	{
		newConacts[A][B] = hit;
	}
}
