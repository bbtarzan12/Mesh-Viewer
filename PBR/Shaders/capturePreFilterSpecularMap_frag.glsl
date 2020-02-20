#version 430 core

#include common.cgin
#include pbr.cgin

in vec3 UV;

out vec4 color;

uniform float roughness;
uniform TextureCube cubeMapTexture;

const uint SAMPLE_COUNT = 1024u;

void main()
{
	vec3 normal = normalize(UV);
	vec3 reflectDir = normal;
	vec3 viewDir = normal;

	vec3 preFilteredColor = vec3(0);
	float totalWeight = 0.0;

	for (uint i = 0u; i < SAMPLE_COUNT; i++)
	{
		vec2 Xi = Hammersley(i, SAMPLE_COUNT);
		vec3 halfDir = ImportanceSampleGGX(Xi, normal, roughness);
		vec3 lightDir = normalize(2 * dot(viewDir, halfDir) * halfDir - viewDir);

		float NdotL = saturate(dot(normal, lightDir));
		if (NdotL > 0)
		{
			preFilteredColor += Sample(cubeMapTexture, lightDir).rgb * NdotL;
			totalWeight += NdotL;
		}
	}

	preFilteredColor = preFilteredColor / totalWeight;
	color = vec4(preFilteredColor, 1);
}
