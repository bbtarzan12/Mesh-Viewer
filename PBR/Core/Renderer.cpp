#include "Renderer.h"
#include "../Managers/GLFWManager.h"
#include "../Core/Camera.h"
#include "../Core/Mesh.h"
#include "../Managers/MaterialManager.h"
#include "../Materials/Phong.h"
#include "../Util/TextureLoader.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

Renderer::Renderer(const RendererOption& rendererOption)
	:rendererOption(rendererOption)
{

}

Renderer::~Renderer()
{

}

void Renderer::Init()
{
	GLFWManager::Instance().Init(rendererOption.width, rendererOption.height, "PBR", this);

	std::shared_ptr<Phong> phongMaterial = MaterialManager::Instance().CreateMaterial<Phong>("Phong");
	phongMaterial->SetDiffuseTexture(TextureLoader::Load("Images/Stone_02_COLOR.png"));
	phongMaterial->SetNormalTexture(TextureLoader::Load("Images/Stone_02_NRM.png"));
	phongMaterial->SetSpecularTexture(TextureLoader::Load("Images/Stone_02_SPEC.png"));
	phongMaterial->SetVec3("lightPosition_worldSpace", glm::vec3(7, 7, 7));

	std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>("Meshes/sphere.obj");
	mesh->SetMaterial(phongMaterial);

	meshes.push_back(mesh);
}

void Renderer::SetCamera(const std::shared_ptr<Camera>& newCamera)
{
	camera = newCamera;
	camera->UpdateScreen(rendererOption.width, rendererOption.height);
}

void Renderer::Start()
{
	double lastTime = glfwGetTime();
	while (GLFWManager::WindowShouldClose() == 0)
	{
		glfwPollEvents();

		double currentTime = glfwGetTime();
		deltaTime = currentTime - lastTime;
		lastTime = currentTime;

		Update(deltaTime);
		Render(deltaTime);
	}
}

void Renderer::Release()
{
	GLFWManager::Instance().Release();
}

void Renderer::Render(double deltaTime)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	for (auto& mesh : meshes)
	{
		mesh->GetMaterial()->SetVec3("cameraPosition_worldSpace", camera->GetPosition());
		mesh->Draw(camera->GetView(), camera->GetProjection());
	}

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	glfwMakeContextCurrent(GLFWManager::GetWindow());
	glfwSwapBuffers(GLFWManager::GetWindow());
}

void Renderer::Update(double deltaTime)
{
	glm::vec2 input = glm::vec2(GLFWManager::IsKeyDown(GLFW_KEY_W) ? 1 : GLFWManager::IsKeyDown(GLFW_KEY_S) ? -1 : 0, GLFWManager::IsKeyDown(GLFW_KEY_D) ? 1 : GLFWManager::IsKeyDown(GLFW_KEY_A) ? -1 : 0);
	camera->UpdateCamera(deltaTime, input);
}

void Renderer::HandleKeyboard(int key, int scancode, int action, int mods)
{

}

void Renderer::HandleMouseClick(int button, int action, int mods)
{
	if (GLFWManager::IsMouseDown(GLFW_MOUSE_BUTTON_MIDDLE))
	{
		GLFWManager::SetCursorToPos(rendererOption.width / 2, rendererOption.height / 2);
		glfwSetInputMode(GLFWManager::GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
	else
	{
		glfwSetInputMode(GLFWManager::GetWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
}

void Renderer::HandleMouseMotion(double xPos, double yPos)
{
	if (GLFWManager::IsMouseDown(GLFW_MOUSE_BUTTON_MIDDLE))
	{
		int& width = rendererOption.width;
		int& height = rendererOption.height;

		GLFWManager::SetCursorToPos(width / 2, height / 2);
		glm::vec2 input = glm::vec2(width / 2 - xPos, height / 2 - yPos);
		camera->UpdateCamera(deltaTime, glm::vec2(0), input);
		GLFWManager::SetCursorToPos(width / 2, height / 2);
	}
}

void Renderer::HandleResize(int width, int height)
{
	rendererOption.width = width;
	rendererOption.height = height;

	camera->UpdateScreen(width, height);
	glViewport(0, 0, width, height);
}

