#include "FrameBuffer.h"
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include "../Util/DrawHelper.h"
#include "../Textures/CubeMapTexture.h"
#include "../Materials/IrradianceMapCapture.h"

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

void FrameBuffer::Capture(const std::shared_ptr<Texture>& destination, const std::shared_ptr<Material>& material, const int maxMip)
{
	if (std::shared_ptr<CubeMapTexture> cubeMapTexture = std::dynamic_pointer_cast<CubeMapTexture>(destination))
	{
		CaptureCubeMapTexture(cubeMapTexture, material, maxMip);
	}
	else
	{
		CaptureTexture(destination, material, maxMip);
	}
}

void FrameBuffer::Capture(const std::shared_ptr<Texture>& source, const std::shared_ptr<Texture>& destination, const std::shared_ptr<Material>& material, const int maxMip)
{
	material->SetCaptureTexture(source);
	Capture(destination, material, maxMip);
}

void FrameBuffer::Resize(const glm::ivec2& size)
{
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, size.x, size.y);
	glViewport(0, 0, size.x, size.y);
	this->size = size;
}

void FrameBuffer::CaptureTexture(const std::shared_ptr<Texture>& destination, const std::shared_ptr<Material>& material, const int maxMip)
{
	const int& destID = destination->GetID();

	material->Use();

	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	const glm::ivec2& size = destination->GetSize();
	glDisable(GL_CULL_FACE);
	glm::mat4 empty(0);
	
	for (int mip = 0; mip < maxMip; mip++)
	{
		glm::ivec2 mipSize = size;
		material->Capture(mipSize, mip, maxMip);
		Resize(mipSize);
		material->Draw(empty, empty, empty);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, destID, mip);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		DrawHelper::DrawQuad();
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glEnable(GL_CULL_FACE);
}

void FrameBuffer::CaptureCubeMapTexture(const std::shared_ptr<CubeMapTexture>& destination, const std::shared_ptr<Material>& material, const int maxMip)
{
	const int& destID = destination->GetID();

	material->Use();

	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	const glm::ivec2& size = destination->GetSize();
	glDisable(GL_CULL_FACE);
	glm::mat4 model(1.0f);
	for (int mip = 0; mip < maxMip; mip++)
	{
		glm::ivec2 mipSize = size;
		material->Capture(mipSize, mip, maxMip);
		Resize(mipSize);
		for (int i = 0; i < 6; i++)
		{
			material->Draw(model, cubeMapCaptureViews[i], captureProjection);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, destID, mip);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			DrawHelper::DrawCube();
		}
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glEnable(GL_CULL_FACE);
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
