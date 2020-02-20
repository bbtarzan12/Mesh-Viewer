#pragma once

#include <Gl/glew.h>
#include <glm/glm.hpp>
#include <memory>
#include <string>

class Material;
class Texture;
class CubeMapTexture;
class CubeMapCapture;

class FrameBuffer
{
public:
	FrameBuffer(const glm::ivec2& size);
	~FrameBuffer();

	void Capture(const std::shared_ptr<Texture>& destination, const std::shared_ptr<Material>& material);
	void CaptureIrradianceMap(const std::shared_ptr<CubeMapTexture>& source, const std::shared_ptr<CubeMapTexture>& destination, const std::shared_ptr<CubeMapCapture>& cubeMapCapture);
	void CapturePreFilterMap(const std::shared_ptr<CubeMapTexture>& source, const std::shared_ptr<CubeMapTexture>& destination, const std::shared_ptr<CubeMapCapture>& cubeMapCapture, const int maxMipLevel);

private:
	void Resize(const glm::ivec2& size);

private:
	GLuint fbo, rbo;
	glm::ivec2 size;

	// CubeMapCapture
	static glm::mat4 cubeMapCaptureViews[];
	static glm::mat4 captureProjection;
};
