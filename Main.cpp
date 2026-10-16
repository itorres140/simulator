#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include "vec2.h"
#include "particle.h"
#include <vector>

// Vertex Shader with 2D orthographic projection
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec2 aPos;\n"
"uniform vec2 offset;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos + offset, 0.0, 1.0);\n"
"}\0";

// Fragment Shader
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";


// Get the number of segments for the circle from the user
// Ideally wanna make ui for all input instead of using console
int getCircleVertices()
{
	int segments;
	std::cout << "Enter number of segments: ";
	std::cin >> segments;
	return segments;
};

// Get the radius of the circle
float getCircleRadius()
{
	float radius;
	std::cout << "Enter radius: ";
	std::cin >> radius;
	return radius;
};

// Get the initial position of particle
void setInitialPosition(particle& p, float x, float y)
{
	std::cout << "Enter initial x position: ";
	std::cin >> x;
		if (x < -1.0f || x > 1.0f)
		{
			std::cout << "x position must be between -1.0 and 1.0" << std::endl;
			return;
		}
	std::cout << "Enter initial y position: ";
	std::cin >> y;
		if (y < -1.0f || y > 1.0f)
		{
			std::cout << "y position must be between -1.0 and 1.0" << std::endl;
			return;
		}
	p.position = flatVector(x, y);
};

// Get the number of circles from the user
int getNumCircles()
{
	int numCircles;
	std::cout << "Enter number of circles: ";
	std::cin >> numCircles;
	return numCircles;
};

float radius = getCircleRadius();
int segments = getCircleVertices();

std::vector<float> createCircle(float radius = radius, int segments = segments)
{
	std::vector<float> vertices;
	// radius = getCircleRadius();
	// segments = getCircleVertices();

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
};

int main()
{
	
	// Initialize GLFW
	glfwInit();
	
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
	gladLoadGL();


	// let glad execute commands in the window, setting view to 800x800
	glViewport(0, 0, 800, 800);

	// Create vertex shader object and get its reference
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	// Attach the vertex shader source code to the vertex shader object
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	// Compile the source code into machine code
	glCompileShader(vertexShader);

	// Create fragment shader object and get its reference
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	// Attach the fragment source code to the fragment shdaer object
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	// Compile the source code into machine code
	glCompileShader(fragmentShader);

	// Create shader program object and get its reference
	GLuint shaderProgram = glCreateProgram();
	// Attach the vertex and fragment shader to the shader program object
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	// Link the two shaders into the program 
	glLinkProgram(shaderProgram);

	// Delete the vertex shader and fragment shader objects
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);












	// Create circle vertices based on the specified radius and number of segments
	std::vector<float> circleVertices = createCircle(radius, segments);


	// store circle information in particle vector
	std::vector<particle> circles;

	// Add circles as position, radius, mass, and restitution.
	circles.push_back(particle(flatVector(0.0f, 0.0f), 0.08f, 1.0f));

	circles.push_back(particle(flatVector(0.5f, 0.5f), 0.08f, 1.0f));

	circles.push_back(particle(flatVector(-0.5f, -0.5f), 0.08f, 1.0f));

	circles.push_back(particle(flatVector(0.5f, -0.5f), 0.08f, 1.0f));
	circles.push_back(particle(flatVector(-0.5f, 0.5f), 0.08f, 1.0f));















	// Create reference for Vertex array object and Vertex buffer object
	GLuint VAO, VBO;

	// Generate the VAO and VBO with 1 object each
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	// Make VAO the current Vertex Array Object
	glBindVertexArray(VAO);
	
	// Bind VBO making sure its a GL_ARRAY_BUFFER
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Upload the circle mesh, not the particle state.
	glBufferData(GL_ARRAY_BUFFER, circleVertices.size() * sizeof(float), circleVertices.data(), GL_STATIC_DRAW);

	// Configure VAO so that OpenGL knows how to read the VBO
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	// Enable vertex attribute so OpenGL knows what to do with it
	glEnableVertexAttribArray(0);

	// Bind both VAO and VBO to 0
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	GLint offSetLoc = glGetUniformLocation(shaderProgram, "offset");

	// timing
	float lastTime = static_cast<float>(glfwGetTime());


	// Setting the color (R, G, B, A)
	glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glfwSwapBuffers(window);




	// initialize with velocity
	// first circle
	circles[0].velocity = flatVector(0.8f, 0.1f);
	// second circle
	circles[1].velocity = flatVector(-1.0f, 0.1f);
	// third circle
	circles[2].velocity = flatVector(0.1f, -7.0f);
	// fourth circle
	circles[3].velocity = flatVector(-0.1f, -0.1f);
	// fifth circle
	circles[4].velocity = flatVector(0.0f, 0.0f);


	// Main loop, keeps window open until user closes it
	while (!glfwWindowShouldClose(window))
	{

		// find dt
		float currentTime = static_cast<float>(glfwGetTime());
		float dt = currentTime - lastTime;
		lastTime = currentTime;

		// cap the maximum delta time to prevent large jumps
		if (dt > 0.01f) dt = 0.01f;

		// rk4 integration for each circle
		for (particle& circle : circles)
		{
			circle.rk4(dt);
		}

		// collision with left boundary for all circles 
		for (particle& circle : circles)
		{
			if (circle.position.x - circle.radius <= -0.9f)
			{
				circle.position.x = -0.9f + circle.radius;
				circle.velocity = flatVector(-circle.velocity.x * circle.restitution, circle.velocity.y);
			}
		}
		// collision with bottom boundary for all circles
		for (particle& circle : circles)
		{
			if (circle.position.y - circle.radius <= -0.9f)
			{
				circle.position.y = -0.9f + circle.radius;
				circle.velocity = flatVector(circle.velocity.x, -circle.velocity.y * circle.restitution);

			}
		}
		// collision with right boundary for all circles
		for (particle& circle : circles)
		{
			if (circle.position.x + circle.radius >= 0.9f)
			{
				circle.position.x = 0.9f - circle.radius;
				circle.velocity = flatVector(-circle.velocity.x * circle.restitution, circle.velocity.y);
			}
		}

		// collision with top boundary for all circles
		for (particle& circle : circles)
		{
			if (circle.position.y + circle.radius > 0.9f)
			{
				circle.position.y = 0.9f - circle.radius;
				circle.velocity = flatVector(circle.velocity.x, -circle.velocity.y * circle.restitution);
			}
		}

		// collision between circles for all pairs



















		glClear(GL_COLOR_BUFFER_BIT);
		// Tell OpenGL which Shader Program we want to use
		glUseProgram(shaderProgram);
		// Bind the VAO so OpenGL knows to use it
		glBindVertexArray(VAO);
		// Draw the triangles using the function GL_TRIANGLES
		for (const particle& circle : circles)
		{
			glUniform2f(offSetLoc, circle.position.x, circle.position.y);
			glDrawArrays(GL_TRIANGLE_FAN, 0,
				static_cast<GLsizei>(circleVertices.size() / 2));
		}
		glfwSwapBuffers(window);

		glfwPollEvents();
	}


	// Deleting the VAO, VBO and shader program
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);

	glfwDestroyWindow(window);
	glfwTerminate();
	
	return 0;
}