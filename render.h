
#pragma once

#include <glad/glad.h>
#include <vector>
#include "particle.h"

class Renderer
{
public:
    Renderer(float circleRadius, int segments);
    ~Renderer();

    // Prevent accidental copying of OpenGL resources
    Renderer(const Renderer&) = delete;
    Renderer& operator=(const Renderer&) = delete;

    void drawCircles(const std::vector<particle>& circles);
    void drawRectangles(const std::vector<particle>& rectangles);

private:
    GLuint shaderProgram = 0;

    GLuint circleVAO = 0;
    GLuint circleVBO = 0;

    GLuint rectangleVAO = 0;
    GLuint rectangleVBO = 0;

    GLint offsetLoc = -1;

    GLsizei circleVertexCount = 0;

    void createShaderProgram();
    void createCircleMesh(float radius, int segments);
    void createRectangleMesh();
};
