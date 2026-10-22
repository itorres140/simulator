
#include "render.h"

#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <stdexcept>

namespace
{
    const char* vertexShaderSource = R"(
        #version 330 core

        layout (location = 0) in vec2 aPos;
        uniform vec2 offset;

        void main()
        {
            gl_Position = vec4(aPos + offset, 0.0, 1.0);
        }
    )";

    const char* fragmentShaderSource = R"(
        #version 330 core

        out vec4 FragColor;

        void main()
        {
            FragColor = vec4(1.0, 0.5, 0.2, 1.0);
        }
    )";

    GLuint compileShader(GLenum type, const char* source)
    {
        GLuint shader = glCreateShader(type);
        glShaderSource(shader, 1, &source, nullptr);
        glCompileShader(shader);

        GLint success = GL_FALSE;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

        if (!success)
        {
            GLint length = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);

            std::string log(length > 0 ? length : 1, '\0');
            glGetShaderInfoLog(
                shader, length, nullptr, log.data());

            std::cerr << "Shader compilation failed:\n"
                      << log << '\n';

            glDeleteShader(shader);
            return 0;
        }

        return shader;
    }
}

Renderer::Renderer(float circleRadius, int segments)
{
    createShaderProgram();
    createCircleMesh(circleRadius, segments);
    createRectangleMesh();

    offsetLoc = glGetUniformLocation(shaderProgram, "offset");
}

Renderer::~Renderer()
{
    // This destructor must run while the OpenGL context is alive.
    glDeleteVertexArrays(1, &circleVAO);
    glDeleteBuffers(1, &circleVBO);

    glDeleteVertexArrays(1, &rectangleVAO);
    glDeleteBuffers(1, &rectangleVBO);

    glDeleteProgram(shaderProgram);
}

void Renderer::createShaderProgram()
{
    GLuint vertexShader =
        compileShader(GL_VERTEX_SHADER, vertexShaderSource);

    GLuint fragmentShader =
        compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

    if (vertexShader == 0 || fragmentShader == 0)
    {
        if (vertexShader != 0)
            glDeleteShader(vertexShader);

        if (fragmentShader != 0)
            glDeleteShader(fragmentShader);

        throw std::runtime_error("Failed to compile shaders");
    }

    shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    GLint success = GL_FALSE;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    if (!success)
    {
        GLint length = 0;
        glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &length);

        std::string log(length > 0 ? length : 1, '\0');
        glGetProgramInfoLog(
            shaderProgram, length, nullptr, log.data());

        std::cerr << "Shader linking failed:\n"
                  << log << '\n';

        glDeleteProgram(shaderProgram);
        shaderProgram = 0;

        throw std::runtime_error("Failed to link shader program");
    }
}

void Renderer::createCircleMesh(float radius, int segments)
{
    if (segments < 3 || radius <= 0.0f)
        throw std::invalid_argument("Invalid circle dimensions");

    std::vector<float> vertices;

    constexpr float PI = 3.14159265358979323846f;

    // Center vertex
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);

    // Circle perimeter
    for (int i = 0; i <= segments; ++i)
    {
        float angle = 2.0f * PI * i / segments;

        vertices.push_back(std::cos(angle) * radius);
        vertices.push_back(std::sin(angle) * radius);
    }

    circleVertexCount =
        static_cast<GLsizei>(vertices.size() / 2);

    glGenVertexArrays(1, &circleVAO);
    glGenBuffers(1, &circleVBO);

    glBindVertexArray(circleVAO);
    glBindBuffer(GL_ARRAY_BUFFER, circleVBO);

    glBufferData(
        GL_ARRAY_BUFFER,
        vertices.size() * sizeof(float),
        vertices.data(),
        GL_STATIC_DRAW
    );

    glVertexAttribPointer(
        0, 2, GL_FLOAT, GL_FALSE,
        2 * sizeof(float), nullptr
    );

    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Renderer::createRectangleMesh()
{
    // Reserve space for 4 vertices (8 floats).
    // The actual rectangle dimensions are uploaded when drawing.
    glGenVertexArrays(1, &rectangleVAO);
    glGenBuffers(1, &rectangleVBO);

    glBindVertexArray(rectangleVAO);
    glBindBuffer(GL_ARRAY_BUFFER, rectangleVBO);

    glBufferData(
        GL_ARRAY_BUFFER,
        8 * sizeof(float),
        nullptr,
        GL_DYNAMIC_DRAW
    );

    glVertexAttribPointer(
        0, 2, GL_FLOAT, GL_FALSE,
        2 * sizeof(float), nullptr
    );

    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Renderer::drawCircles(
    const std::vector<particle>& circles)
{
    glUseProgram(shaderProgram);
    glBindVertexArray(circleVAO);

    for (const particle& circle : circles)
    {
        glUniform2f(
            offsetLoc,
            circle.position.x,
            circle.position.y
        );

        glDrawArrays(
            GL_TRIANGLE_FAN,
            0,
            circleVertexCount
        );
    }

    glBindVertexArray(0);
}

void Renderer::drawRectangles(
    const std::vector<particle>& rectangles)
{
    glUseProgram(shaderProgram);
    glBindVertexArray(rectangleVAO);
    glBindBuffer(GL_ARRAY_BUFFER, rectangleVBO);

    for (const particle& rectangle : rectangles)
    {
        const float halfWidth = rectangle.width / 2.0f;
        const float halfHeight = rectangle.height / 2.0f;

        // Actual dimensions, centered on the particle position.
        const float vertices[] = {
            -halfWidth, -halfHeight,
             halfWidth, -halfHeight,
             halfWidth,  halfHeight,
            -halfWidth,  halfHeight
        };

        glBufferSubData(
            GL_ARRAY_BUFFER,
            0,
            sizeof(vertices),
            vertices
        );

        glUniform2f(
            offsetLoc,
            rectangle.position.x,
            rectangle.position.y
        );

        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    }

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
