#version 430 core
layout(location = 0) in vec3 vertexPosition_modelSpace;
layout(location = 1) in vec2 vertexUV;

out vec2 UV;

void main()
{
	UV = vertexUV;
	gl_Position = vec4(vertexPosition_modelSpace, 1.0);
}