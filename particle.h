#pragma once
#include "vec2.h"

class particle
{
public:
	flatVector position;
	flatVector velocity;
	flatVector acceleration;
	float mass;
	float radius;

	particle()
		: position(0.0f, 0.0f),
		velocity(0.0f, 0.0f),
		acceleration(0.0f, 0.0f),
		mass(1.0f),
		radius(0.05f)
	{
	}

	particle(flatVector pos, float r, float m)
		: position(pos),
		velocity(0.0f, 0.0f),
		acceleration(0.0f, 0.0f),
		mass(m),
		radius(r)
	{
	}

	void applyForce(flatVector force)
	{
		acceleration = acceleration + (force / mass);
	}

	void applyGravity(float gravity)
	{
		applyForce(flatVector(0.0f, -gravity * mass));
	}

	void rk4(float dt)
	{
		flatVector k1position = velocity;
		flatVector k1velocity = acceleration;

		flatVector k2position = velocity + k1velocity * (dt * 0.5f);
		flatVector k2velocity = acceleration;

		flatVector k3position = velocity + k2velocity * (dt * 0.5f);
		flatVector k3velocity = acceleration;

		flatVector k4position = velocity + k3velocity * dt;
		flatVector k4velocity = acceleration;

		position = position + (k1position + k2position * 2.0f + k3position * 2.0f + k4position) * (dt / 6.0f);
		velocity = velocity + (k1velocity + k2velocity * 2.0f + k3velocity * 2.0f + k4velocity) * (dt / 6.0f);
	}
};

