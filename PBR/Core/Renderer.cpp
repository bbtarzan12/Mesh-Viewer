#include "Renderer.h"
#include "../Managers/GLFWManager.h"
#include "../Core/Camera.h"
#include "../Core/Mesh.h"
#include "../Managers/MaterialManager.h"
#include "../Materials/CubeMap.h"
#include "../Materials/Phong.h"
#include "../Materials/PBR.h"
#include "../Util/TextureLoader.h"
#include "Texture.h"
#include "../Textures/CubeMapTexture.h"
#include "../Lights/PointLight.h"
#include "../Lights/DirectionalLight.h"

#include <imgui.h>
#include <imfilebrowser.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_custom.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>


Renderer::Renderer(const RendererOption& rendererOption)
	:rendererOption(rendererOption), fileDialog(std::make_shared<ImGui::FileBrowser>())
{

}

Renderer::~Renderer()
{

}

void Renderer::Init()
{
	GLFWManager::Instance().Init(rendererOption.width, rendererOption.height, "PBR", this);

	// #Todo : CubeMap Mesh�� Material �������� �����ϱ�
	std::vector<std::string> faces
	{
		"Images/Environment/nx.png",
		"Images/Environment/px.png",
		"Images/Environment/py.png",
		"Images/Environment/ny.png",
		"Images/Environment/pz.png",
		"Images/Environment/nz.png"
	};

	std::shared_ptr<CubeMapTexture> cubeMapTexture = std::make_shared<CubeMapTexture>(faces, true);

	std::shared_ptr<CubeMap> cubeMapMaterial = MaterialManager::Instance().CreateMaterial<CubeMap>("CubeMap");
	cubeMapMaterial->SetTexture("cubeMapTexture", cubeMapTexture);

	LoadMesh("Meshes/skybox.obj", "CubeMap");

	// #Todo : Material ���� ������ �����
	std::shared_ptr<Phong> phongMaterial = MaterialManager::Instance().CreateMaterial<Phong>("Phong");
	phongMaterial->SetTexture("cubeMapTexture", cubeMapTexture);
	phongMaterial->SetTexture("diffuseTexture", std::make_shared<Texture>("Images/Stone_02_COLOR.png", true));
	phongMaterial->SetTexture("normalTexture", std::make_shared<Texture>("Images/Stone_02_NRM.png"));
	phongMaterial->SetTexture("specularTexture", std::make_shared<Texture>("Images/Stone_02_SPEC.png"));

	std::shared_ptr<PBR> pbrMaterial = MaterialManager::Instance().CreateMaterial<PBR>("PBR");
	pbrMaterial->SetTexture("albedoTexture", std::make_shared<Texture>("Images/chipped-paint-metal-albedo.png", true));
	pbrMaterial->SetTexture("normalTexture", std::make_shared<Texture>("Images/chipped-paint-metal-normal-dx.png"));
	pbrMaterial->SetTexture("metallicTexture", std::make_shared<Texture>("Images/chipped-paint-metal-metal.png"));
	pbrMaterial->SetTexture("roughnessTexture", std::make_shared<Texture>("Images/chipped-paint-metal-rough2.png"));
	pbrMaterial->SetTexture("aoTexture", std::make_shared<Texture>("Images/chipped-paint-ao.png"));

	std::shared_ptr<DirectionalLight> directionalLight = std::make_shared<DirectionalLight>(glm::vec3(0, -1, 0), glm::vec3(1, 1, 1), 1.0f);
	std::shared_ptr<PointLight> light = std::make_shared<PointLight>(glm::vec3(13, 0, 0), glm::vec3(1, 1, 1), 10.0f);
	std::shared_ptr<PointLight> light2 = std::make_shared<PointLight>(glm::vec3(13, 0, 6), glm::vec3(1, 1, 1), 10.0f);
	std::shared_ptr<PointLight> light3 = std::make_shared<PointLight>(glm::vec3(13, 7, 0), glm::vec3(1, 1, 1), 10.0f);
	std::shared_ptr<PointLight> light4 = std::make_shared<PointLight>(glm::vec3(13, 10, 6), glm::vec3(1, 1, 1), 10.0f);
	lights.push_back(directionalLight);
	lights.push_back(light);
	lights.push_back(light2);
	lights.push_back(light3);
	lights.push_back(light4);
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

	for (auto& mesh : meshes)
	{
		const std::shared_ptr<Material> material = mesh->GetMaterial();
		for(int i = 0 ; i < lights.size(); i++)
		{
			lights[i]->Draw(material, i);
		}

		material->SetVec3("cameraPosition_worldSpace", camera->GetPosition());
		mesh->Draw(camera->GetView(), camera->GetProjection());
	}

	RenderUI(deltaTime);

	glfwMakeContextCurrent(GLFWManager::GetWindow());
	glfwSwapBuffers(GLFWManager::GetWindow());
}

void Renderer::RenderUI(double deltaTime)
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::MenuItem("Load Mesh"))
		{
			bEnableMeshLoadWindow = !bEnableMeshLoadWindow;

			fileDialog->SetTitle("Load Mesh");
			fileDialog->SetTypeFilters({ ".obj" });
			fileDialog->Open();
		}

		if (ImGui::MenuItem("Meshes"))
		{
			bEnableMeshesWindow = !bEnableMeshesWindow;
		}

		if (ImGui::MenuItem("Material"))
		{
			bEnableMaterialWindow = !bEnableMaterialWindow;
		}
		ImGui::EndMainMenuBar();
	}

	if (bEnableMeshesWindow)
	{
		RenderMeshesWindow(deltaTime);
	}

	if (bEnableMaterialWindow)
	{
		RenderMaterialWindow(deltaTime);
	}

	if (bEnableMeshLoadWindow)
	{
		RenderMeshLoadWindow(deltaTime);
	}


	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Renderer::RenderMaterialWindow(double deltaTime)
{
	ImGui::Begin("Material", &bEnableMaterialWindow, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize);

	if (selectedMeshIndex == -1)
	{
		ImGui::TextDisabled("Selected Mesh == nullptr");
		ImGui::End();
		return;
	}

	meshes[selectedMeshIndex]->GetMaterial()->DrawUI();

	ImGui::End();
}

void Renderer::RenderMeshesWindow(double deltaTime)
{
	ImGui::Begin("Meshes", &bEnableMeshesWindow, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize);

	if (meshes.empty())
	{
		ImGui::TextDisabled("meshes.size() == 0");
		ImGui::End();
		return;
	}

	std::vector<std::string> meshNameList;
	meshNameList.reserve(meshes.size());
	for (auto& mesh : meshes)
	{
		meshNameList.push_back(mesh->GetName());
	}

	ImGui::ListBox("##Mesh List", &selectedMeshIndex, meshNameList);

	if (selectedMeshIndex >= 0)
	{
		float width = ImGui::GetWindowContentRegionWidth();
		ImGui::BeginChild(meshes[selectedMeshIndex]->GetName().c_str(), { width, 150 }, true);

		meshes[selectedMeshIndex]->DrawUI();

		if (ImGui::Button("Remove Mesh"))
		{
			meshes.erase(meshes.begin() + selectedMeshIndex);
			
			selectedMeshIndex = -1;
		}

		ImGui::EndChild();
	}

	ImGui::End();
}

void Renderer::RenderMeshLoadWindow(double deltaTime)
{
	if (bEnableSetMaterialWhenMeshLoad)
	{
		ImGui::SetNextWindowPos({ rendererOption.width / 2.0f, rendererOption.height / 2.0f }, ImGuiCond_Always, { 0.5f, 0.5f });
		ImGui::Begin("Load Mesh", &bEnableSetMaterialWhenMeshLoad, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar);

		int numMaterials = MaterialManager::Instance().NumMaterials();

		if (numMaterials <= 0)
		{
			if (ImGui::Button("New Material"))
			{
				// #Todo : Material ���� ������ �����
			}
		}
		else
		{
			std::vector<std::string> names;
			names.reserve(numMaterials);
			for (auto& material : MaterialManager::Instance().GetMaterials())
			{
				names.push_back(material.first);
			}

			ImGui::Text("Material List");
			if (ImGui::ListBox("##Material List", &selectedMaterialIndex, names))
			{
				LoadMesh(selectedMeshFilePath, names[selectedMaterialIndex]);
				selectedMaterialIndex = -1;
				bEnableSetMaterialWhenMeshLoad = false;
				bEnableMeshLoadWindow = false;
			}
		}
		ImGui::End();
	}
	else
	{
		fileDialog->Display();
		if (fileDialog->HasSelected())
		{
			selectedMeshFilePath = fileDialog->GetSelected().string();
			bEnableSetMaterialWhenMeshLoad = true;
			fileDialog->ClearSelected();
		}
	}
}

void Renderer::Update(double deltaTime)
{
	glm::vec2 input = glm::vec2(GLFWManager::IsKeyDown(GLFW_KEY_W) ? 1 : GLFWManager::IsKeyDown(GLFW_KEY_S) ? -1 : 0, GLFWManager::IsKeyDown(GLFW_KEY_D) ? 1 : GLFWManager::IsKeyDown(GLFW_KEY_A) ? -1 : 0);
	camera->UpdateCamera(deltaTime, input);
}

void Renderer::LoadMesh(const std::string& filePath, const std::string& materialName)
{
	std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(filePath);
	mesh->SetMaterial(MaterialManager::Instance().GetMaterial(materialName));

	meshes.push_back(mesh);
}

void Renderer::LoadCubeMap(const std::vector<std::string>& faces)
{

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

