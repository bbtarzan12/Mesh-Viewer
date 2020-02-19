#pragma once

#include <GL/glew.h>
#include <vector>

class DrawHelper
{
public:
	static void DrawCube();

private:
	static GLuint cubeVAO;
	static GLuint cubeVBO;
	static std::vector<float> cubeVertices;
};
