#version 430 core

#include common.cgin

in vec3 UV;

out vec4 color;

uniform TextureCube cubeMapTexture;

void main()
{
	vec3 normal = normalize(UV);
	vec3 irradiance = vec3(0);

	vec3 up = vec3(0, 1, 0);
	vec3 right = cross(up, normal);
	up = cross(normal, right);

	float deltaPhi = 0.025;
	float deltaTheta = 0.025;
	float numSamples = 0;
	for (float phi = 0; phi < 2.0 * PI; phi += deltaPhi)
	{
		for (float theta = 0; theta < 0.5 * PI; theta += deltaTheta)
		{
			float sinTheta = sin(theta);
			float cosTheta = cos(theta);
			float cosPhi = cos(phi);
			float sinPhi = sin(phi);
			
			vec3 cartesian = vec3(sinTheta * cosPhi, sinTheta * sinPhi, cosTheta);
			vec3 worldDirection = cartesian.x * right + cartesian.y * up + cartesian.z * normal;

			irradiance += Sample(cubeMapTexture, worldDirection).rgb * cos(theta) * sin(theta);
			numSamples += 1;
		}
	}

	irradiance = PI * irradiance * (1.0 / numSamples);
	color = vec4(irradiance, 1);
}
