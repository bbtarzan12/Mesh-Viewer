#include "Core/Camera.h"
#include "Core/Renderer.h"

int main()
{
	int width = 1280;
	int height = 720;

	RendererOption renderOption;
	renderOption.width = width;
	renderOption.height = height;

	std::unique_ptr<Renderer> renderer = std::make_unique<Renderer>(renderOption);
	std::shared_ptr<Camera> camera = std::make_shared<Camera>(Camera({ width, height }, 70.0f, { 0.01f, 1000.0f }));

	renderer->Init();
	renderer->SetCamera(camera);
	renderer->Start();
	renderer->Release();

	return 0;
}