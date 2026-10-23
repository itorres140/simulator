#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include "vec2.h"
#include "particle.h"
#include <vector>
#include <random>
#include "render.h"
#include "shapes.h"
using namespace std;

random_device rd;
mt19937 gen(rd());

float randomFloat(float min, float max)
{
	std::uniform_real_distribution<float> dis(min, max);
	return dis(gen);
}

flatVector randomPosition()
{
	float x = randomFloat(-0.8, 0.8);
	float y = randomFloat(-0.8, 0.8);
	return flatVector(x, y);
}

flatVector randomVelocity()
{
	return flatVector(randomFloat(-0.5f, 0.5f), randomFloat(-0.5f, 0.5f));
}

flatVector randomAcceleration()
{
	return flatVector(randomFloat(-0.2f, 0.2f), randomFloat(-0.2f, 0.2f));
}
// Get the number of segments for the circle from the user
// Ideally wanna make ui for all input instead of using console

// Get the radius of the circle
float getCircleRadius()
{
	float radius;
	std::cout << "Enter radius: ";
	std::cin >> radius;

	if (radius <= 0.0f)
	{
		std::cout << "Radius must be greater than 0" << std::endl;
		return getCircleRadius();
	}

	if (radius != radius) // Check for NaN
	{
		std::cout << "Radius must be a valid number" << std::endl;
		return getCircleRadius();
	}
	return radius;
};

// Get the initial position of particle
flatVector setInitialPosition()
{

	float x;
	float y;
	
	std::cout << "Enter initial x position: ";
	std::cin >> x;
		if (x < -1.0f || x > 1.0f)
		{
			std::cout << "x position must be between -1.0 and 1.0" << std::endl;
			return setInitialPosition();
		};

	std::cout << "Enter initial y position: ";
	std::cin >> y;
		if (y < -1.0f || y > 1.0f)
		{
			std::cout << "y position must be between -1.0 and 1.0" << std::endl;
			return setInitialPosition();

		};

	return flatVector(x, y);
}; 

int getNumCircles()
{
	int numCircles;
	std::cout << "Enter number of circles: ";
	std::cin >> numCircles;
	return numCircles;
};

// Creating random width and height for the rectangle
float getRandomWidth()
{
	float width = randomFloat(0.0f, 1.0f);
	return width;
}

float getRandomHeight()
{
	float height = randomFloat(0.0f, 1.0f);
	return height;
};

void handleWallCollision(vector<particle>& circle, vector<particle>& rectangle)
{
	// Handle collision for circle particle
	for (particle& one : circle)
	{
		if (one.shape == 0) // circle
	{
			if (one.position.x + one.radius > 1.0f || one.position.x - one.radius < -1.0f)
			{
				one.velocity.x *= -1.0f;
			}
			if (one.position.y + one.radius > 1.0f || one.position.y - one.radius < -1.0f)
			{
				one.velocity.y *= -1.0f;
			}
		}
	}

	for (particle& one : rectangle)
	{
		if (one.shape == 1) // rectangle
		{
			if (one.position.x + one.width / 2.0f > 1.0f || one.position.x - one.width / 2.0f < -1.0f)
			{
				one.velocity.x *= -1.0f;
			}
			if (one.position.y + one.height / 2.0f > 1.0f || one.position.y - one.height / 2.0f < -1.0f)
			{
				one.velocity.y *= -1.0f;
			}
		}
	}
};


//Creating main function for program
int main()
{


	float radius = getCircleRadius();
	int segments = 50;
	int numCircles = getNumCircles();

	int numRectangles = 3;
	float height = getRandomHeight();
	float width = getRandomWidth();


	// Initialize GLFW
	if (!glfwInit())
	{
		std::cout << "Failed to initialize GLFW" << std::endl;
		return -1;
	}
	
	// Tell GLFW version of OpenGL is being used, 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Core profile being used, only modern functions
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	

	// Making a window, 800x800
	GLFWwindow* window = glfwCreateWindow(800, 800, "OpenGL Window", NULL, NULL);
	// Checking for error in making window
	if (window == NULL)
	{
		std::cout << "Failed to create window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// Making window the context for openGL so that commands are executed in this window
	glfwMakeContextCurrent(window);

	// Load glad
	if (!gladLoadGL())
	{
		std::cout << "Failed to load GLAD" << std::endl;
		glfwTerminate();
		return -1;
	}


	// let glad execute commands in the window, setting view to 800x800
	glViewport(0, 0, 800, 800);
	glClearColor(0.07f, 0.13f, 0.17f, 1.0f);

	vector<particle> circles;

	for (int i = 0; i < numCircles; i++)
	{
		// random position, velocity, and acceleration per circle
		flatVector pos = randomPosition();
		flatVector vel = randomVelocity();
		flatVector acc = randomAcceleration();

		// create a new particle with the random values
		circles.emplace_back(pos, radius, 1.0f);

		// set the velocity and acceleration for the newly created particle
		circles.back().velocity = vel;
		circles.back().acceleration = acc;
	};



	vector<particle> rectangles;
		
	for (int i = 0; i < numRectangles; i++)
		{
			shapeType type = rectangle;

			// random position, velocity, and acceleration per rectangle
			flatVector pos = randomPosition();
			flatVector vel = randomVelocity();
			flatVector acc = randomAcceleration();

			// create a new particle with the random values
			rectangles.emplace_back(pos, 1.0f, 1.0f, type);

			// set the velocity and acceleration for the newly created particle
			rectangles.back().velocity = vel;
			rectangles.back().acceleration = acc;

			rectangles.back().width = randomFloat(0.2f, 0.5f);
			rectangles.back().height = randomFloat(0.1f, 0.3f);
		};
	// Setting the color (R, G, B, A)
	glClear(GL_COLOR_BUFFER_BIT);
	glfwSwapBuffers(window);
	{
		Renderer render(radius, segments);
		// Main loop, keeps window open until user closes it
		while (!glfwWindowShouldClose(window))
		{
			float dt = .02f; // cap the delta time to avoid large jumps

			// rk4 integration for each circle
			for (particle& circle : circles)
			{
				circle.rk4(dt);
			}

			for (particle& rectangle : rectangles)
			{
				rectangle.rk4(dt);
			}
			
			handleWallCollision(circles, rectangles);

			glClear(GL_COLOR_BUFFER_BIT);
			render.drawCircles(circles);
			render.drawRectangles(rectangles);
			glfwSwapBuffers(window);

			glfwPollEvents();
		};
	};

	glfwDestroyWindow(window);
	glfwTerminate();
	
	return 0;
}