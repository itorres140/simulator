#pragma once
#include "vec2.h"
#include <tuple>
#include <vector>

// storing all particle prop and state into vector
struct particle
{

	flatVector position;
	flatVector velocity;
	flatVector acceleration;
	flatVector angularVelocity;
	float mass;
	float radius;
	float restitution;
	float angle;
	float area;
	float height;
	float density;
	float volume;
	float width;
	float length;
	int id;

	particle(flatVector pos,float r, float m, float e = 0.8f):
		position(pos),
		velocity(0.0f, 0.0f),
		acceleration(0.0f, -9.81f),
		angularVelocity(0.0f, 0.0f),
		mass(m != 0.0f ? m : 1.0f),
		radius(r),
		restitution(e),
		angle(0.0f),
		area(0.0f),
		height(0.0f),
		density(0.0f),
		volume(0.0f),
		width(0.0f),
		length(0.0f),
		id(0)
	
	{
	}
// intend on moving this to be a function in a sim class
	std::tuple<float, float, float, float> rk4(float dt)
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

		return std::make_tuple(position.x, position.y, velocity.x, velocity.y);
	}

	public:
		std::vector<particle> getProperties() const
	{
		return {*this};
	}

};

