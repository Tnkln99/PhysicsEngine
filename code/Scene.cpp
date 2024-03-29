//
//  Scene.cpp
//
#include "Scene.h"
#include "../Shape.h"
#include "../Intersections.h"


/*
========================================================================================================

Scene

========================================================================================================
*/

/*
====================================================
Scene::~Scene
====================================================
*/
Scene::~Scene() {
	for ( int i = 0; i < bodies.size(); i++ ) {
		delete bodies[ i ].shape;
	}
	bodies.clear();
}

/*
====================================================
Scene::Reset
====================================================
*/
void Scene::Reset() {
	for ( int i = 0; i < bodies.size(); i++ ) {
		delete bodies[ i ].shape;
	}
	bodies.clear();

	Initialize();
}

/*
====================================================
Scene::Initialize
====================================================
*/
void Scene::Initialize() {
	float posOffsetZ = 5;
	for(int i = 0; i < 3; ++i)
	{
		float posOffsetY = 0;
		for (int i = 0; i < 3; ++i)
		{
			float posOffsetX = 0;
			for (int i = 0; i < 3; ++i)
			{
				Body body;
				body.position = Vec3(posOffsetX, posOffsetY, posOffsetZ);
				body.orientation = Quat(0, 0, 0, 1);
				body.shape = new ShapeSphere(1.0f);
				body.inverseMass = 1.0f;
				body.elasticity = 1.0f;
				body.friction = 0.6f;
				body.linearVelocity = Vec3(0, 1, 0);
				bodies.push_back(body);
				posOffsetX += 5;
			}
			posOffsetY += 5;
		}
		posOffsetZ += 5;
	}

	Body earth;
	earth.position = Vec3(0, 0, -1000);
	earth.orientation = Quat(0, 0, 0, 1);
	earth.shape = new ShapeSphere(1000.0f);
	earth.inverseMass = 0.0f;
	earth.elasticity = 0.99f;
	earth.friction = 0.5f;
	bodies.push_back(earth);
}

/*
====================================================
Scene::Update
====================================================
*/
void Scene::Update( const float dt_sec ) {
	// TODO: Add code

	// Collision checks
	for (int i = 0; i < bodies.size(); ++i)
	{
		for (int j = i + 1; j < bodies.size(); ++j)
		{
			Body& bodyA = bodies[i];
			Body& bodyB = bodies[j];
			if (bodyA.inverseMass == 0.0f && bodyB.inverseMass == 0.0f)
				continue;
			Contact contact;
			if (Intersections::Intersect(bodyA, bodyB, contact))
			{
				Contact::ResolveContact(contact);
			}
		}
	}

	// Position update
	for (int i = 0; i < bodies.size(); ++i) {
		Body& body = bodies[i];
		float mass = 1.0f / body.inverseMass;
		// Gravity needs to be an impulse I
		// I == dp, so F == dp/dt <=> dp = F * dt
		// <=> I = F * dt <=> I = m * g * dt
		Vec3 impulseGravity = Vec3(0, 0, -10) * mass * dt_sec;
		body.ApplyImpulseLinear(impulseGravity);
	}
	// Position update
	for (int i = 0; i < bodies.size(); ++i) {
		bodies[i].Update(dt_sec);
	}
}