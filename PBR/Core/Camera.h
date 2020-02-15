#pragma once

#include <glm/glm.hpp>

class Camera
{
public:
	Camera(const glm::uvec2& size, float fov, const glm::vec2& clippingPlanes);

	void UpdateScreen(int width, int height);
	void UpdateCamera(float deltaTime, const glm::vec2& keyboard = glm::vec2(0, 0), const glm::vec2& mouse = glm::vec2(0, 0));
	void HandleInput(float deltaTime, int key);

	const glm::vec3& GetPosition() const;
	const glm::mat4& GetView() const;
	const glm::mat4& GetProjection() const;

private:
	glm::vec3 position;
	glm::mat4 proj;
	glm::mat4 view;

	glm::vec3 forward, up, right;

	float fov;
	float aspectRatio;

	glm::vec2 clippingPlanes;
	glm::uvec2 size;

	float pitch, yaw;
	float moveSpeed, mouseSpeed;
};