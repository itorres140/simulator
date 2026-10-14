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

// vector creation functions for shapes

// for circle
std::vector<float> createCircle(int segments)
{
	std::vector<float> vertices;

	const float PI = 3.14159265358979323846f;
	const float radius = 0.08f;

	for (int i = 0; i <= segments; i++)
	{
		float angle = 2.0f * PI * i / segments;

		vertices.push_back(std::cos(angle) * radius);
		vertices.push_back(std::sin(angle) * radius);
	}
	return vertices;
}

// for rectangle
std::vector<float> createRectangle(float width, float height)
{
	std::vector<float> vertices;

	vertices.push_back(-width / 2.0f);
	vertices.push_back(-height / 2.0f);
	vertices.push_back(width / 2.0f);
	vertices.push_back(-height / 2.0f);
	vertices.push_back(width / 2.0f);
	vertices.push_back(height / 2.0f);
	vertices.push_back(-width / 2.0f);
	vertices.push_back(height / 2.0f);

	return vertices;
}

// combined vertex data for shapes generated
std::vector<float> circleVertices = createCircle(60);
std::vector<float> rectangleVertices = createRectangle(0.2f, 0.1f);

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
	// Make circle geometry
	std::vector<float> circleVertices = createCircle(60);

	// create rectangle geometry
	std::vector<float> rectangleVertices = createRectangle(0.2f, 0.1f);
	// Create VAO and VBO for rectangle
	GLuint rectangleVAO, rectangleVBO;
	glGenVertexArrays(1, &rectangleVAO);
	glGenBuffers(1, &rectangleVBO);

	glBindVertexArray(rectangleVAO);
	glBindBuffer(GL_ARRAY_BUFFER, rectangleVBO);
	glBufferData(GL_ARRAY_BUFFER, rectangleVertices.size() * sizeof(float), rectangleVertices.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// Create reference for Vertex array object and Vertex buffer object
	GLuint VAO, VBO;

	// Generate the VAO and VBO with 1 object each
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	// Make VAO the current Vertex Array Object
	glBindVertexArray(VAO);
	
	// Bind VBO making sure its a GL_ARRAY_BUFFER
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Introduce the verticies into the VBO
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


	
	// initiliaze ball particle with (position, radius, mass))
	particle ball(flatVector(-0.3f, 0.7f), 0.08f, 1.0f);
	// set inital angle
	float angleRadians = -30.0f * (3.14159265f / 180.0f);
	// set inital speed
	float speed = 1.5f;
	// set initial velocity based on angle and speed
	ball.velocity = flatVector(speed * cos(angleRadians), speed * sin(angleRadians));

	// initalize rectangle with (position, width, height)
	particle rectangle(flatVector(0.0f, -0.5f), 0.2f, 0.1f);
	// set initial velocity for rectangle
	rectangle.velocity = flatVector(0.0f, 0.0f);
	// set initial restitution for rectangle
	rectangle.restitution = 0.8f;
	// set initial mass for rectangle
	rectangle.mass = 1.0f;
		
	// Main loop, keeps window open until user closes it
	while (!glfwWindowShouldClose(window))
	{

		// find dt
		float currentTime = static_cast<float>(glfwGetTime());
		float dt = currentTime - lastTime;
		lastTime = currentTime;

		if (dt > 0.01f) dt = 0.01f;

		// physics
		//-----------
		ball.rk4(dt);
		rectangle.rk4(dt);

		// collision with left boundary
		if (ball.position.x - ball.radius <= -0.9f)
		{
			ball.position.x = -0.9f + ball.radius;
			ball.velocity = flatVector(-ball.velocity.x * ball.restitution, ball.velocity.y);
		}
		// collision with bottom boundary
		if (ball.position.y - ball.radius <= -0.9f)
		{
			ball.position.y = -0.9f + ball.radius;
			ball.velocity = flatVector(ball.velocity.x, -ball.velocity.y * ball.restitution);

		}
		// collision with right boundary
		if (ball.position.x + ball.radius >= 0.9f)
		{
			ball.position.x = 0.9f - ball.radius;
			ball.velocity = flatVector(-ball.velocity.x * ball.restitution, ball.velocity.y);
		}

		// collision with top boundary
		if (ball.position.y + ball.radius > 0.9f)
		{
			ball.position.y = 0.9f - ball.radius;
			ball.velocity = flatVector(ball.velocity.x, -ball.velocity.y * ball.restitution);
		}

		// rectangle collisions
		// left
		if (rectangle.position.x - rectangle.width / 2 <= -0.9f)
		{
			rectangle.position.x = -0.9f + rectangle.width / 2;
			rectangle.velocity = flatVector(-rectangle.velocity.x * rectangle.restitution, rectangle.velocity.y);
		}
		// right
		if (rectangle.position.x + rectangle.width / 2 >= 0.9f)
		{
			rectangle.position.x = 0.9f - rectangle.width / 2;
			rectangle.velocity = flatVector(-rectangle.velocity.x * rectangle.restitution, rectangle.velocity.y);
		}
		// top
		if (rectangle.position.y + rectangle.height / 2 >= 0.9f)
		{
			rectangle.position.y = 0.9f - rectangle.height / 2;
			rectangle.velocity = flatVector(rectangle.velocity.x, -rectangle.velocity.y * rectangle.restitution);
		}
		// bottom
		if (rectangle.position.y - rectangle.height / 2 <= -0.9f)
		{
			rectangle.position.y = -0.9f + rectangle.height / 2;
			rectangle.velocity = flatVector(rectangle.velocity.x, -rectangle.velocity.y * rectangle.restitution);
		}


		glClear(GL_COLOR_BUFFER_BIT);
		// Tell OpenGL which Shader Program we want to use
		glUseProgram(shaderProgram);
		glUniform2f(offSetLoc, ball.position.x, ball.position.y);
		// Bind the VAO so OpenGL knows to use it
		glBindVertexArray(VAO);
		// Draw the triangles using the function GL_TRIANGLES
		glDrawArrays(GL_TRIANGLE_FAN, 0, circleVertices.size() / 2);
		glUniform2f(offSetLoc, rectangle.position.x, rectangle.position.y);
		glBindVertexArray(rectangleVAO);
		glDrawArrays(GL_TRIANGLE_FAN, 0, rectangleVertices.size() / 2);
		glfwSwapBuffers(window);

		glfwPollEvents();
	}


	// Deleting the VAO, VBO and shader program
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &rectangleVAO);
	glDeleteBuffers(1, &rectangleVBO);
	glDeleteProgram(shaderProgram);

	glfwDestroyWindow(window);
	glfwTerminate();
	
	return 0;
}