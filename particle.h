#pragma once
#include "vec2.h"
#include <tuple>
#include <vector>
#include <cmath>


enum shapeType
{
	circle,
	rectangle
};
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
	shapeType shape;


	particle(flatVector pos, float m, float e = 0.8f, shapeType type = circle):
		position(pos),
		velocity(0.0f, 0.0f),
		acceleration(0.0f, -9.81f),
		angularVelocity(0.0f, 0.0f),
		mass(m != 0.0f ? m : 1.0f),
		radius(0.0f),
		restitution(e),
		angle(0.0f),
		area(0.0f),
		height(0.0f),
		density(0.0f),
		volume(0.0f),
		width(0.0f),
		length(0.0f),
		shape(type)
	{}

	flatVector calcAccel(float dt)
	{
		acceleration = acceleration * dt;
		return acceleration;
	}
// intend on moving this to be a function in a sim class
	std::tuple<float, float, float, float> rk4(float dt)
	{
		flatVector k1_pos = velocity;
		flatVector k1_vel = calcAccel(dt);

		flatVector k2_pos = velocity + k1_vel * (dt * 0.5f);
		flatVector k2_vel = calcAccel(dt);

		flatVector k3_pos = velocity + k2_vel * (dt * 0.5f);
		flatVector k3_vel = calcAccel(dt);

		flatVector k4_pos = velocity + k3_vel * dt;
		flatVector k4_vel = calcAccel(dt);

		position = position + (k1_pos + (k2_pos * 2.0f) + (k3_pos * 2.0f) + k4_pos) * (dt / 6.0f);
		velocity = velocity + (k1_vel +  (k2_vel * 2.0f) + (k3_vel * 2.0f) + k4_vel) * (dt / 6.0f);

		return std::make_tuple(position.x, position.y, velocity.x, velocity.y);
	}

};



