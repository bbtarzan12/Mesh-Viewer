#version 430 core

#include common.cgin
#include pbr.cgin

in vec2 UV;

out vec4 color;

const uint SAMPLE_COUNT = 1024u;

void main()
{
	float NdotV = UV.x;
	float roughness = UV.y;

	vec3 viewDir = vec3(sqrt(1.0 - NdotV * NdotV), 0, NdotV);

	float a = 0;
	float b = 0;

	vec3 normal = vec3(0, 0, 1);

	for (int i = 0; i < SAMPLE_COUNT; i++)
	{
		vec2 Xi = Hammersley(i, SAMPLE_COUNT);
		vec3 halfDir = ImportanceSampleGGX(Xi, normal, roughness);
		vec3 lightDir = normalize(2.0 * dot(viewDir, halfDir) * halfDir - viewDir);

		float NdotL = saturate(lightDir.z);
		float NdotH = saturate(halfDir.z);
		float VdotH = saturate(dot(viewDir, halfDir));

		if (NdotL > 0.0)
		{
			float G = GeometrySmithsMethodIBLSpecular(normal, viewDir, lightDir, roughness);
			float G_Vis = (G * VdotH) / (NdotH * NdotV);
			float Fc = pow(1.0 - VdotH, 5.0);

			a += (1.0 - Fc) * G_Vis;
			b += Fc * G_Vis;
		}
	}

	a /= float(SAMPLE_COUNT);
	b /= float(SAMPLE_COUNT);

	color = vec4(a, b, 0, 1);
}
