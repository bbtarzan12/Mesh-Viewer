#version 330 core

layout(location = 0) in vec3 vertexPosition_modelSpace;
layout(location = 1) in vec3 vertexNormal_modelSpace;
layout(location = 2) in vec2 vertexUV;
layout(location = 3) in vec3 tangent_modelSpace;

out vec2 UV;
out vec3 fragPosition_worldSpace;
out vec3 fragPosition_tangentSpace;
out vec3 cameraPosition_tangentSpace;
out vec3 lightPosition_tangentSpace;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;
uniform vec3 lightPosition_worldSpace;
uniform vec3 cameraPosition_worldSpace;

void main()
{
	fragPosition_worldSpace = (M * vec4(vertexPosition_modelSpace, 1)).xyz;
	UV = vertexUV;

	mat3 normalMatrix = transpose(inverse(mat3(M)));
	vec3 T = normalize(normalMatrix * tangent_modelSpace);
	vec3 N = normalize(normalMatrix * vertexNormal_modelSpace);
	T = normalize(T - dot(T, N) * N);
	vec3 B = cross(N, T);

	mat3 TBN = transpose(mat3(T, B, N));
	fragPosition_tangentSpace = TBN * fragPosition_worldSpace;
	cameraPosition_tangentSpace = TBN * cameraPosition_worldSpace;
	lightPosition_tangentSpace = TBN * lightPosition_worldSpace;

	gl_Position = P * V * M * vec4(vertexPosition_modelSpace, 1.0);
}