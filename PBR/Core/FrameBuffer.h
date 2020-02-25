#pragma once

#include <Gl/glew.h>
#include <glm/glm.hpp>
#include <memory>
#include <string>

class Material;
class Texture;
class CubeMapTexture;
class IrradianceMapCapture;

class FrameBuffer
{
public:
	FrameBuffer(const glm::ivec2& size = glm::ivec2(1, 1));
	~FrameBuffer();

	void Capture(const std::shared_ptr<Texture>& destination, const std::shared_ptr<Material>& material, const int maxMip = 1);
	void Capture(const std::shared_ptr<Texture>& source, const std::shared_ptr<Texture>& destination, const std::shared_ptr<Material>& material, const int maxMip = 1);

private:
	void Resize(const glm::ivec2& size);
	void CaptureTexture(const std::shared_ptr<Texture>& destination, const std::shared_ptr<Material>& material, const int maxMip);
	void CaptureCubeMapTexture(const std::shared_ptr<CubeMapTexture>& destination, const std::shared_ptr<Material>& material, const int maxMip);

private:
	GLuint fbo, rbo;
	glm::ivec2 size;

	// CubeMapCapture
	static glm::mat4 cubeMapCaptureViews[];
	static glm::mat4 captureProjection;
};
