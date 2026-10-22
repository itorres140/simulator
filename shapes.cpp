#include "shapes.h"
#include <cmath>

std::vector<float> createCircle(float radius, int segments)
{
	std::vector<float> vertices;
	const float PI = 3.14159265358979323846f;

	vertices.push_back(0.0f);
	vertices.push_back(0.0f);

	for (int i = 0; i <= segments; i++)
	{
		float angle = 2.0f * PI * i / segments;

		vertices.push_back(std::cos(angle) * radius);
		vertices.push_back(std::sin(angle) * radius);
	}
	return vertices;
}

std::vector<float> createRectangle(float width, float height)
{
	std::vector<float> vertices;
	vertices.push_back(-width / 2);
	vertices.push_back(-height / 2);
	vertices.push_back(width / 2);
	vertices.push_back(-height / 2);
	vertices.push_back(width / 2);
	vertices.push_back(height / 2);
	vertices.push_back(-width / 2);
	vertices.push_back(height / 2);
	return vertices;
}