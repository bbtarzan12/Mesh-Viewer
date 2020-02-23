#version 430 core

#include common.cgin
#include pbr.cgin

in vec2 UV;
in vec3 fragPosition_worldSpace;
in vec3 fragPosition_tangentSpace;
in vec3 cameraPosition_tangentSpace;
in vec3 vertexNormal_tangentSpace;
in vec3 lightPositions_tangentSpace[NUM_LIGHTS];
in mat3 TBN;

out vec3 color;

uniform TextureCube irradianceMapTexture;
uniform TextureCube preFilterSpecularMapTexture;
uniform Texture2D brdfSpecularMapTexture;
uniform Texture2D albedoTexture;
uniform Texture2D normalTexture;
uniform Texture2D metallicTexture;
uniform Texture2D roughnessTexture;
uniform Texture2D aoTexture;

uniform vec3 cameraPosition_worldSpace;
uniform vec2 uvOffset;
uniform vec2 uvScale;
uniform int ndfIndex;
uniform int gIndex;
uniform int fIndex;

uniform Light lights[NUM_LIGHTS];

void main()
{

	vec2 uv = vec2(UV.x * uvScale.x, UV.y * uvScale.y) + uvOffset;
	vec3 normal = normalize(vertexNormal_tangentSpace);
	if (normalTexture.use)
	{
		normal = normalize((Sample(normalTexture, uv).rgb - 0.5) * 2.0);
	}

	vec3 albedo = Sample(albedoTexture, uv).rgb;
	float metallic = Sample(metallicTexture, uv).r;
	float roughness = Sample(roughnessTexture, uv).r;
	float ao = Sample(aoTexture, uv).r;

	vec3 F0 = vec3(0.04);
	F0 = mix(F0, albedo, metallic);
	vec3 Lo = vec3(0);

	vec3 viewDir = normalize(cameraPosition_tangentSpace - fragPosition_tangentSpace);
	vec3 reflectDir = normalize(reflect(-viewDir, normal));
	for (int i = 0; i < NUM_LIGHTS; i++)
	{
		vec3 lightDir = lights[i].isDirectional ?
			normalize(-lightPositions_tangentSpace[i]) :
			normalize(lightPositions_tangentSpace[i] - fragPosition_tangentSpace);
		vec3 halfDir = normalize(lightDir + viewDir);

		float distance = length(lights[i].position - fragPosition_worldSpace);
		float attenuation = lights[i].isDirectional ? 1.0 : 1.0 / (distance * distance);
		vec3 radiance = lights[i].color * lights[i].power * attenuation;

		float NDF = NormalDistributionFunction(ndfIndex, normal, halfDir, roughness);
		float G = GeometryFunction(gIndex, normal, viewDir, lightDir, roughness);
		vec3 F = FresnelFunction(fIndex, F0, viewDir, halfDir);

		float NdotL = saturate(dot(normal, lightDir));
		float NdotV = saturate(dot(normal, viewDir));

		vec3 num = NDF * G * F;
		float denom = 4.0 * NdotL * NdotV + Epsilon;
		vec3 specular = num / denom;

		vec3 kS = F;
		vec3 kD = vec3(1.0) - kS;
		kD *= 1.0 - metallic;

		Lo += (kD * albedo / PI + specular) * radiance * saturate(dot(normal, lightDir));
	}

	vec3 F = FresnelSchlickApproximationWithRoughness(F0, viewDir, normal, roughness);

	vec3 kS = F;
	vec3 kD = vec3(1.0) - kS;
	kD *= 1.0 - metallic;

	vec3 irradiance = Sample(irradianceMapTexture, normal * TBN).rgb;
	vec3 diffuse = irradiance * albedo;

	const float MAX_REFLECTION_LOD = 5.0 - 1.0;
	vec3 prefilteredColor = SampleLod(preFilterSpecularMapTexture, reflectDir * TBN, roughness * MAX_REFLECTION_LOD).rgb;
	vec2 brdf = Sample(brdfSpecularMapTexture, vec2(saturate(dot(normal, viewDir)), roughness)).rg;
	vec3 specular = prefilteredColor * (F * brdf.x + brdf.y);

	vec3 ambient = (kD * diffuse + specular) * ao;

	color = GammaCorrection(ambient + Lo);
}