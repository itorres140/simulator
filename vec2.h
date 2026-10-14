#pragma once

#include <cmath>

// 2d vector arithmetic and operations class

class flatVector
{	
public:
	float x;
	float y;

	flatVector() : x(0.0f), y(0.0f) {}
	flatVector(float xVal, float yVal) : x(xVal), y(yVal) {}

	// Vector addition
	flatVector operator+(const flatVector& other) const
		{
		return { x + other.x, y + other.y };
		}
	
	// Vector subtraction
	flatVector operator-(const flatVector& other) const
		{
		return flatVector(x - other.x, y - other.y);
		}

	// Scalar multiplication
	flatVector operator*(float scalar) const 
	{
		return flatVector(x * scalar, y * scalar );
	}

	// Scalar division
	flatVector operator/(float scalar) const
	{
		return flatVector(x / scalar, y / scalar);
	}
	
	// Dot product
	float dot(const flatVector& other) const
	{
		return x * other.x + y * other.y;
	}

	// Magnitude
	float magnitude() const
	{
		return std::sqrt(dot(*this));
	}

	// Normalized
	flatVector norm() const
	{
		float mag = std::sqrt(dot(*this));

		if (mag == 0.0f)
			return flatVector(0.0f, 0.0f);

		return *this / mag;
	}
};
