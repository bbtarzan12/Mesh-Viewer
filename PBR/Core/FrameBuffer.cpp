#include "FrameBuffer.h"
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include "../Util/DrawHelper.h"
#include "../Textures/CubeMapTexture.h"
#include "../Materials/CubeMapCapture.h"

FrameBuffer::FrameBuffer(const glm::ivec2& size)
	:size(size)
{
	glGenFramebuffers(1, &fbo);
	glGenRenderbuffers(1, &rbo);

	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, size.x, size.y);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

FrameBuffer::~FrameBuffer()
{
	glDeleteFramebuffers(1, &fbo);
	glDeleteRenderbuffers(1, &rbo);
}

void FrameBuffer::CaptureIrradianceMap(const std::shared_ptr<CubeMapTexture> source, const std::shared_ptr<CubeMapTexture> destination, const std::shared_ptr<CubeMapCapture>& cubeMapCapture)
{
	const int& sourceID = source->GetID();
	const int& destID = destination->GetID();

	cubeMapCapture->SetTexture("cubeMapTexture", source);
	cubeMapCapture->Use();

	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	Resize({ size.x, size.y });
	glDisable(GL_CULL_FACE);
	glm::mat4 model(1.0f);
	for (int i = 0; i < 6; i++)
	{
		cubeMapCapture->Draw(model, cubeMapCaptureViews[i], captureProjection);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, destID, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		DrawHelper::DrawCube();
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glEnable(GL_CULL_FACE);
}

void FrameBuffer::CapturePreFilterMap(const std::shared_ptr<CubeMapTexture> source, const std::shared_ptr<CubeMapTexture> destination, const std::shared_ptr<CubeMapCapture>& cubeMapCapture, const int maxMipLevel)
{
	const int& sourceID = source->GetID();
	const int& destID = destination->GetID();

	cubeMapCapture->SetTexture("cubeMapTexture", source);
	cubeMapCapture->Use();

	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	glDisable(GL_CULL_FACE);
	glm::mat4 model(1.0f);

	for (int mip = 0; mip < maxMipLevel; mip++)
	{
		int mipWidth = destination->GetSize().x * std::pow(0.5, mip);
		int mipHeight = destination->GetSize().y * std::pow(0.5, mip);
		Resize({ mipWidth, mipHeight });

		float roughness = mip / (float)(maxMipLevel - 1);
		cubeMapCapture->SetFloat("roughness", roughness);
		for (int i = 0; i < 6; i++)
		{
			cubeMapCapture->Draw(model, cubeMapCaptureViews[i], captureProjection);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, destID, mip);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			DrawHelper::DrawCube();
		}
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glEnable(GL_CULL_FACE);
}

void FrameBuffer::Resize(const glm::ivec2& size)
{
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, size.x, size.y);
	glViewport(0, 0, size.x, size.y);
}

glm::mat4 FrameBuffer::captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);

glm::mat4 FrameBuffer::cubeMapCaptureViews[]
{
	glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
	glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
	glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
	glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
	glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
	glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
};
