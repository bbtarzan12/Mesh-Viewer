#include "Camera.h"

#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>


Camera::Camera(const glm::uvec2& size, float fov, const glm::vec2& clippingPlanes) :
	position(10, 0, 10),
	up(0, 1, 0),
	forward(0, 0, -1),
	right(1, 0, 0),
	yaw(0.0f),
	pitch(90.0f),
	size(size),
	fov(fov),
	clippingPlanes(clippingPlanes),
	aspectRatio(size.x / (float)size.y),
	moveSpeed(10.0f),
	mouseSpeed(20.0f)
{
	UpdateCamera(0.01f, { 0.001f, 0.001f }, {0.001f, 0.001f});
}

void Camera::UpdateScreen(int width, int height)
{
	size.x = width;
	size.y = height;
	aspectRatio = width / (float)height;
}

void Camera::UpdateCamera(float deltaTime, const glm::vec2& keyboard /*= glm::vec2(0, 0)*/, const glm::vec2& mouse /*= glm::vec2(0, 0)*/)
{
	if (glm::length(keyboard) == 0 && glm::length(mouse) == 0)
		return;

	pitch -= mouseSpeed * mouse.y * deltaTime;
	yaw += mouseSpeed * mouse.x * deltaTime;

	pitch = glm::clamp(pitch, 1.0f, 179.0f); // theta
	yaw = glm::mod(yaw, 360.0f); // phi

	position += (forward * keyboard.x + right * keyboard.y) * moveSpeed * deltaTime;
	forward.x = glm::sin(glm::radians(pitch)) * glm::sin(glm::radians(yaw));
	forward.y = glm::cos(glm::radians(pitch));
	forward.z = glm::sin(glm::radians(pitch)) *glm::cos(glm::radians(yaw));
	forward = glm::normalize(forward);
	right = glm::normalize(glm::cross(forward, glm::vec3(0, 1, 0)));
	up = glm::normalize(glm::cross(right, forward));

	view = glm::lookAt(position, position + forward, up);
	proj = glm::perspective(glm::radians(fov), aspectRatio, clippingPlanes.x, clippingPlanes.y);
}

void Camera::HandleInput(float deltaTime, int key)
{

}

const glm::vec3& Camera::GetPosition() const
{
	return position;
}

const glm::mat4& Camera::GetView() const
{
	return view;
}

const glm::mat4& Camera::GetProjection() const
{
	return proj;
}
