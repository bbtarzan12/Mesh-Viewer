#pragma once

#include <vector>
#include <memory>

class Camera;
class Mesh;
class Light;
class CubeMapTexture;

struct RendererOption
{
	int width, height;
};

class Renderer
{

public:
	Renderer(const RendererOption& rendererOption);
	~Renderer();

	void Init();
	void SetCamera(const std::shared_ptr<Camera>& newCamera);
	void Start();
	void Release();

	void HandleKeyboard(int key, int scancode, int action, int mods);
	void HandleMouseClick(int button, int action, int mods);
	void HandleMouseMotion(double xPos, double yPos);
	void HandleResize(int width, int height);

private:
	void Render(double deltaTime);
	void Update(double deltaTime);
	void LoadMesh(const std::string& filePath, const std::string& materialName);
	void LoadCubeMap(const std::vector<std::string>& faces);

	void CreateMaterial();
	void CreateLight();

	// UI
	void RenderUI(double deltaTime);
	void RenderMaterialWindow(double deltaTime);
	void RenderMeshesWindow(double deltaTime);
	void RenderMeshLoadWindow(double deltaTime);

private:
	double deltaTime;
	RendererOption rendererOption;
	std::shared_ptr<Camera> camera;
	std::vector<std::shared_ptr<Mesh>> meshes;
	std::vector<std::shared_ptr<Light>> lights;

	// Global Environment


	// UI
	int selectedMeshIndex = -1;
	int selectedMaterialIndex = -1;

	std::string selectedMeshFilePath = "";

	bool bEnableMaterialWindow = false;
	bool bEnableMeshesWindow = false;
	bool bEnableMeshLoadWindow = false;
	bool bEnableSetMaterialWhenMeshLoad = false;

};