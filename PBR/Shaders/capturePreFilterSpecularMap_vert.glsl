#version 430 core
layout(location = 0) in vec3 vertexPosition_modelSpace;

out vec3 UV;

uniform mat4 P;
uniform mat4 V;

void main()
{
	UV = vertexPosition_modelSpace;
	gl_Position = P * V * vec4(vertexPosition_modelSpace, 1.0);
}