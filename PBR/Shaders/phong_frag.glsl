#version 430 core

#include common.cgin
#include phong_lights.cgin

in vec2 UV;
in vec3 fragPosition_worldSpace;
in vec3 fragPosition_tangentSpace;
in vec3 cameraPosition_tangentSpace;
in vec3 vertexNormal_tangentSpace;
in vec3 directionalLightDirection_tangentSpace;
in vec3 lightPositions_tangentSpace[NUM_POINT_LIGHTS];
in mat3 TBN;

out vec3 color;

uniform TextureCube cubeMapTexture;
uniform Texture2D diffuseTexture;
uniform Texture2D normalTexture;
uniform Texture2D specularTexture;
uniform vec3 cameraPosition_worldSpace;
uniform vec2 uvOffset;
uniform vec2 uvScale;

uniform DirectionalLight directionalLight;
uniform PointLight pointLights[NUM_POINT_LIGHTS];

vec3 CalculateDirectionalLightDiffuse(DirectionalLight light, vec2 uv, vec3 normal, vec3 lightTangentDirection);
vec3 CalculateDirectionalLightSpecular(DirectionalLight light, vec2 uv, vec3 normal, vec3 lightTangentDirection, vec3 cameraTangentDirection);

vec3 CalculatePointLightDiffuse(PointLight light, vec2 uv, vec3 normal, vec3 lightDir, float attenuation);
vec3 CalculatePointLightSpecular(PointLight light, vec2 uv, vec3 normal, vec3 lightDir, vec3 cameraTangentDirection, float attenuation);

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
	
	vec3 lightDir = normalize(-directionalLightDirection_tangentSpace);
	vec3 cameraTangentDirection = normalize(cameraPosition_tangentSpace - fragPosition_tangentSpace);

	vec3 diffuseLight = CalculateDirectionalLightDiffuse(directionalLight, uv, normal, lightDir);
	vec3 specularLight = CalculateDirectionalLightSpecular(directionalLight, uv, normal, lightDir, cameraTangentDirection);

	for (int i = 0; i < NUM_POINT_LIGHTS; i++)
	{
		float distance = length(pointLights[i].position - fragPosition_worldSpace);
		float attenuation = 1.0 / (distance * distance);

		lightDir = normalize(lightPositions_tangentSpace[i] - fragPosition_tangentSpace);
		diffuseLight += CalculatePointLightDiffuse(pointLights[i], uv, normal, lightDir, attenuation);
		specularLight += CalculatePointLightSpecular(pointLights[i], uv, normal, lightDir, cameraTangentDirection, attenuation);
	}

	// Reflection
	vec3 Ir = reflect(-cameraTangentDirection, normal);
	float FrIr = FresnelSchlickApproximation(clamp(dot(cameraTangentDirection, normal), 0, 1), normal, Ir);
	Ir = Ir * TBN; // Tangent To World space
	vec3 cubeReflection = vec3(Sample(cubeMapTexture, Ir)) * FrIr;

	vec3 specular = vec3(Sample(specularTexture, uv)) * (specularLight + cubeReflection);
	vec3 diffuse = vec3(Sample(diffuseTexture, uv)) * diffuseLight;
	vec3 ambient = vec3(Sample(diffuseTexture, uv)) * 0.1;

	color = diffuse + specular + ambient;
	//color = vec3(Sample(diffuseTexture, uv));
}

vec3 CalculateDirectionalLightDiffuse(DirectionalLight light, vec2 uv, vec3 normal, vec3 lightDir)
{
	float diff = clamp(dot(normal, lightDir), 0, 1);
	return light.color * light.power * diff;
}

vec3 CalculateDirectionalLightSpecular(DirectionalLight light, vec2 uv, vec3 normal, vec3 lightDir, vec3 cameraTangentDirection)
{
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(clamp(dot(cameraTangentDirection, reflectDir), 0, 1), 32);
	return light.color * light.power * spec * FresnelSchlickApproximation(clamp(dot(lightDir, reflectDir), 0, 1), normal, reflectDir);
}

vec3 CalculatePointLightDiffuse(PointLight light, vec2 uv, vec3 normal, vec3 lightDir, float attenuation)
{
	float diff = clamp(dot(normal, lightDir), 0, 1);
	return light.color * light.power * diff * attenuation;
}

vec3 CalculatePointLightSpecular(PointLight light, vec2 uv, vec3 normal, vec3 lightDir, vec3 cameraTangentDirection, float attenuation)
{
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(clamp(dot(cameraTangentDirection, reflectDir), 0, 1), 32);
	return light.color * light.power * spec * attenuation * FresnelSchlickApproximation(clamp(dot(lightDir, reflectDir), 0, 1), normal, reflectDir);
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
	float NdotL = clamp(dot(normal, l), 0, 1);
	return F0 + (1.0 - F0) * pow(1.0 - NdotL, 5);
}