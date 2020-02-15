#pragma once

#include <vector>
#include <memory>

class Camera;
class Mesh;

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

private:
	double deltaTime;
	RendererOption rendererOption;
	std::shared_ptr<Camera> camera;
	std::vector<std::shared_ptr<Mesh>> meshes;

};