#version 330 core

layout(location = 0) in vec3 vertexPosition_modelSpace;
layout(location = 1) in vec3 vertexNormal_modelSpace;
layout(location = 2) in vec2 vertexUV;
layout(location = 3) in vec3 tangent_modelSpace;

#include common.cgin

out vec2 UV;
out vec3 fragPosition_worldSpace;
out vec3 fragPosition_tangentSpace;
out vec3 cameraPosition_tangentSpace;
out vec3 vertexNormal_tangentSpace;
out vec3 lightPositions_tangentSpace[NUM_LIGHTS];
out mat3 TBN;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;
uniform vec3 lightPosition_worldSpace;
uniform vec3 cameraPosition_worldSpace;

uniform Light lights[NUM_LIGHTS];

void main()
{
	fragPosition_worldSpace = (M * vec4(vertexPosition_modelSpace, 1)).xyz;
	UV = vertexUV;

	mat3 normalMatrix = transpose(inverse(mat3(M)));
	vec3 T = normalize(normalMatrix * tangent_modelSpace);
	vec3 N = normalize(normalMatrix * vertexNormal_modelSpace);
	T = normalize(T - dot(T, N) * N);
	vec3 B = cross(N, T);

	TBN = transpose(mat3(T, B, N));
	vertexNormal_tangentSpace = TBN * N;
	fragPosition_tangentSpace = TBN * fragPosition_worldSpace;
	cameraPosition_tangentSpace = TBN * cameraPosition_worldSpace;
	for (int i = 0; i < NUM_LIGHTS; i++)
	{
		lightPositions_tangentSpace[i] = TBN * lights[i].position;
	}

	gl_Position = P * V * M * vec4(vertexPosition_modelSpace, 1.0);
}