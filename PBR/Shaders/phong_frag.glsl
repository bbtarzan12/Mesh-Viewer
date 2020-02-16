#version 330 core

#include phong_lights.cgin

in vec2 UV;
in vec3 fragPosition_worldSpace;
in vec3 fragPosition_tangetSpace;
in vec3 cameraPosition_tangentSpace;
in vec3 lightPositions_tangentSpace[NUM_POINT_LIGHTS];
in vec3 directionalLightDirection_tangentSpace;

out vec3 color;

uniform sampler2D diffuseTexture;
uniform sampler2D normalTexture;
uniform sampler2D specularTexture;
uniform vec3 cameraPosition_worldSpace;
uniform vec2 uvOffset;
uniform vec2 uvScale;

uniform DirectionalLight directionalLight;
uniform PointLight pointLights[NUM_POINT_LIGHTS];

vec3 CalculateDirectionalLight(DirectionalLight light, vec2 uv, vec3 normal, vec3 lightTangentDirection, vec3 cameraTangentDirection);
vec3 CalculatePointLight(PointLight light, vec2 uv, vec3 normal, vec3 lightTangentPosition, vec3 fragWorldPosition, vec3 cameraWorldPosition, vec3 fragTangentPosition, vec3 cameraTangentDirection);

void main()
{
	vec2 uv = vec2(UV.x * uvScale.x, UV.y * uvScale.y) + uvOffset;
	vec3 normal = normalize((texture(normalTexture, uv).rgb - 0.5) * 2.0);

	vec3 cameraTangentDirection = normalize(cameraPosition_tangentSpace - fragPosition_tangetSpace);
	vec3 result = CalculateDirectionalLight(directionalLight, uv, normal, directionalLightDirection_tangentSpace, cameraTangentDirection);
	for (int i = 0; i < NUM_POINT_LIGHTS; i++)
	{
		result += CalculatePointLight(pointLights[i], uv, normal, lightPositions_tangentSpace[i], fragPosition_worldSpace, cameraPosition_worldSpace, fragPosition_tangetSpace, cameraTangentDirection);
	}

	vec3 ambient = vec3(texture(diffuseTexture, uv)) * 0.1;
	result += ambient;

	color = result;
}

vec3 CalculateDirectionalLight(DirectionalLight light, vec2 uv, vec3 normal, vec3 lightTangentDirection, vec3 cameraTangentDirection)
{
	vec3 lightDir = normalize(-lightTangentDirection);
	float diff = clamp(dot(normal, lightDir), 0, 1);

	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(clamp(dot(cameraTangentDirection, reflectDir), 0, 1), 32);

	vec3 diffuse = vec3(texture(diffuseTexture, uv)) * light.color * light.power * diff;
	vec3 specular = vec3(texture(specularTexture, uv)) * light.color * light.power * spec;

	return diffuse + specular;
}

vec3 CalculatePointLight(PointLight light, vec2 uv, vec3 normal, vec3 lightTangentPosition, vec3 fragWorldPosition, vec3 cameraWorldPosition, vec3 fragTangentPosition, vec3 cameraTangentDirection)
{
	float distance = length(light.position - fragWorldPosition);
	float attenuation = 1.0 / (distance * distance);

	vec3 lightDir = normalize(lightTangentPosition - fragTangentPosition);
	float diff = clamp(dot(normal, lightDir), 0, 1);

	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(clamp(dot(cameraTangentDirection, reflectDir), 0, 1), 32);

	vec3 diffuse = vec3(texture(diffuseTexture, uv)) * light.color * light.power * diff * attenuation;
	vec3 specular = vec3(texture(specularTexture, uv)) * light.color * light.power * spec * attenuation;

	return diffuse + specular;
}