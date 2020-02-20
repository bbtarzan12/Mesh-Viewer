#pragma once

#include <GL/glew.h>
#include <vector>

class DrawHelper
{
public:
	static void DrawCube();
	static void DrawQuad();

private:
	static GLuint quadVAO;
	static GLuint quadVBO;
	static std::vector<float> quadVertices;

	static GLuint cubeVAO;
	static GLuint cubeVBO;
	static std::vector<float> cubeVertices;
};
