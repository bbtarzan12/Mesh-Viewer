#version 430 core

#include common.cgin

in vec3 UV;

out vec4 color;

uniform TextureCube cubeMapTexture;

void main()
{
	color = Sample(cubeMapTexture, UV);
}
