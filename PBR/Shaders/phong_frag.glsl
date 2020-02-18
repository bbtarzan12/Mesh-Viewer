#version 430 core

#include common.cgin

in vec2 UV;
in vec3 fragPosition_worldSpace;
in vec3 fragPosition_tangentSpace;
in vec3 cameraPosition_tangentSpace;
in vec3 vertexNormal_tangentSpace;
in vec3 lightPositions_tangentSpace[NUM_LIGHTS];
in mat3 TBN;

out vec3 color;

uniform TextureCube cubeMapTexture;
uniform Texture2D diffuseTexture;
uniform Texture2D normalTexture;
uniform Texture2D specularTexture;

uniform vec3 cameraPosition_worldSpace;
uniform vec2 uvOffset;
uniform vec2 uvScale;
uniform int shininess;
uniform bool useBlinnPhong;

uniform Light lights[NUM_LIGHTS];

float CalculateF0(float IOR);
float FresnelSchlickApproximation(float IOR, vec3 normal, vec3 v);

void main()
{
	vec2 uv = vec2(UV.x * uvScale.x, UV.y * uvScale.y) + uvOffset;
	vec3 normal = normalize(vertexNormal_tangentSpace);
	if (normalTexture.use)
	{
		normal = normalize((Sample(normalTexture, uv).rgb - 0.5) * 2.0);
	}

	vec3 diffuseLight = vec3(0);
	vec3 specularLight = vec3(0);
	vec3 viewDir = normalize(cameraPosition_tangentSpace - fragPosition_tangentSpace);
	for (int i = 0; i < NUM_LIGHTS; i++)
	{
		vec3 lightDir = lights[i].isDirectional ?
			normalize(-lightPositions_tangentSpace[i]) :
			normalize(lightPositions_tangentSpace[i] - fragPosition_tangentSpace);

		float distance = length(lights[i].position - fragPosition_worldSpace);
		float attenuation = lights[i].isDirectional ? 1.0 : 1.0 / (distance * distance);

		float diff = saturate(dot(normal, lightDir));
		vec3 diffuse = lights[i].color * lights[i].power * diff * attenuation;

		float spec = 0;
		vec3 reflectDir = reflect(-lightDir, normal);;
		if (useBlinnPhong)
		{
			vec3 halfDir = normalize(lightDir + viewDir);
			spec = pow(saturate(dot(normal, halfDir)), shininess);
		}
		else
		{
			spec = pow(saturate(dot(viewDir, reflectDir)), shininess);
		}
		vec3 specular = lights[i].color * lights[i].power * spec * attenuation * FresnelSchlickApproximation(saturate(dot(lightDir, reflectDir)), normal, reflectDir);

		diffuseLight += diffuse;
		specularLight += specular;
	}

	// Reflection
	vec3 Ir = reflect(-viewDir, normal);
	float FrIr = FresnelSchlickApproximation(saturate(dot(viewDir, normal)), normal, Ir);
	Ir = Ir * TBN; // Tangent To World space
	vec3 cubeReflection = vec3(Sample(cubeMapTexture, Ir)) * FrIr;

	vec3 specular = vec3(Sample(specularTexture, uv)) * (specularLight + cubeReflection);
	vec3 diffuse = vec3(Sample(diffuseTexture, uv)) * diffuseLight;
	vec3 ambient = vec3(Sample(diffuseTexture, uv)) * 0.03;

	color = GammaCorrection(diffuse + specular + ambient);
}

float CalculateF0(float IOR)
{
	float num = IOR - 1.0;
	float denom = IOR + 1.0;
	float f = (num / denom);

	return f * f;
}

float FresnelSchlickApproximation(float IOR, vec3 normal, vec3 l)
{
	float F0 = CalculateF0(IOR);
	float NdotL = saturate(dot(normal, l));
	return F0 + (1.0 - F0) * pow(1.0 - NdotL, 5);
}