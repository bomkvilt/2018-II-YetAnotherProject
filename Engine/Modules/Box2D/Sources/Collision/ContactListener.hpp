#pragma once
#ifndef CONSTACT_LISTENER_HPP
#define CONSTACT_LISTENER_HPP

#include <unordered_map>

#include "Common.hpp"
#include "Box2D/Box2D.h"


class ContactListener : public b2ContactListener
{
public:
	struct FContactData
	{
		b2Body* body;
		b2Body* other;

		FVector Location;
		FVector HitNormal;
	};

public:
	virtual void BeginContact(b2Contact* contact) override;
	virtual void EndContact  (b2Contact* contact) override;

	void Flush();

public:
	std::unordered_map<
		b2Body*,			// a target body
		std::unordered_map<
			b2Body*,		// a body colided with the one
			FContactData	// the contact's state
			>
		> newConacts;

	std::unordered_map<
		b2Body*,			// a target body
		std::vector<b2Body*>// contacted bodies
		> FinishedContacts;

private:
	void SetContactFinished(b2Body* A, b2Body* B);
	void SetContactStatrted(b2Body* A, b2Body* B, b2WorldManifold mf);
};


#endif // !CONSTACT_LISTENER_HPP
