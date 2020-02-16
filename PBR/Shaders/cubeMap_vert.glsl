#version 330 core
layout(location = 0) in vec3 vertexPosition_modelSpace;

out vec3 UV;

uniform mat4 P;
uniform mat4 V;

void main()
{
	UV = vertexPosition_modelSpace;
	vec4 pos = P * V * vec4(vertexPosition_modelSpace, 1.0);
	gl_Position = pos.xyww;
}