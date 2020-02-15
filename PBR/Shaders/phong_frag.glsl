#version 330 core

in vec2 UV;
in vec3 fragPosition_worldSpace;
in vec3 fragPosition_tangetSpace;
in vec3 cameraPosition_tangentSpace;
in vec3 lightPosition_tangentSpace;

out vec3 color;

uniform sampler2D diffuseTexture;
uniform sampler2D normalTexture;
uniform sampler2D specularTexture;
uniform vec3 lightPosition_worldSpace;
uniform vec3 cameraPosition_worldSpace;

void main()
{
	vec3 lightColor = vec3(1, 1, 1);
	float lightPower = 50.0f;

	vec3 materialDiffuseColor = texture(diffuseTexture, UV).rgb;
	vec3 materialAmbientColor = 0.1 * materialDiffuseColor;
	vec3 materialSpecularColor = texture(specularTexture, UV).rgb * 0.3;

	vec3 textureNormal_tangentSpace = normalize(texture(normalTexture, UV).rgb * 2.0 - 1.0);

	float lightDistance = length(lightPosition_worldSpace - fragPosition_worldSpace);
	float lightDistanceSquared = lightDistance * lightDistance;

	vec3 n = textureNormal_tangentSpace;
	vec3 l = normalize(lightPosition_tangentSpace - fragPosition_tangetSpace);
	float cosTheta = clamp(dot(n, l), 0, 1);

	vec3 E = normalize(cameraPosition_tangentSpace - fragPosition_tangetSpace);
	vec3 R = reflect(-l, n);
	float cosAlpha = clamp(dot(E, R), 0, 1);

	vec3 linearColor = 
		materialAmbientColor +
		materialSpecularColor * lightColor * lightPower * pow(cosAlpha, 32) / lightDistanceSquared +
		materialDiffuseColor * lightColor * lightPower * cosTheta / lightDistanceSquared;

	color = linearColor;
}