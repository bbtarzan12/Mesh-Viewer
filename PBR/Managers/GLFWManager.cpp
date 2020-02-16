#include "GLFWManager.h"

#include <imgui/imgui.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "../Core/Renderer.h"
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>


using namespace std;

void GLFWManager::Init(int width, int height, const char* name, void* renderer)
{
	glfwSetErrorCallback(ErrorCallback);
	if (!glfwInit())
	{
		cerr << "[Error] GLFW 초기화 실패" << endl;
		exit(EXIT_FAILURE);
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
	glfwWindowHint(GLFW_SAMPLES, 4);

	GLFWManager::Instance().window = glfwCreateWindow(width, height, name, nullptr, nullptr);
	if (!GLFWManager::GetWindow())
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(GLFWManager::GetWindow());
	glfwSetWindowUserPointer(GLFWManager::GetWindow(), renderer);
	glfwSetInputMode(GLFWManager::GetWindow(), GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetInputMode(GLFWManager::GetWindow(), GLFW_STICKY_MOUSE_BUTTONS, GL_TRUE);

	glfwSetKeyCallback(GLFWManager::GetWindow(), KeyboardCallback);
	glfwSetMouseButtonCallback(GLFWManager::GetWindow(), MouseCallback);
	glfwSetWindowSizeCallback(GLFWManager::GetWindow(), ResizeCallback);
	glfwSetCursorPosCallback(GLFWManager::GetWindow(), MousePosCallback);
	glfwSetCharCallback(GLFWManager::GetWindow(), CharCallback);

	glewExperimental = GL_TRUE;

	GLenum errorCode = glewInit();
	if (GLEW_OK != errorCode)
	{
		cerr << "[Error] GLEW 초기화 실패" << glewGetErrorString(errorCode) << endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	if (!GLEW_VERSION_4_3)
	{
		cerr << "[Error] 4.3 API가 유효하지 않습니다" << endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	cout << "[OpenGL] OpenGL version: " << glGetString(GL_VERSION) << endl;
	cout << "[OpenGL] GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
	cout << "[OpenGL] Vendor: " << glGetString(GL_VENDOR) << endl;
	cout << "[OpenGL] Renderer: " << glGetString(GL_RENDERER) << endl;

	glClearColor(0.1f, 0.1f, 0.1f, 0.0f);
	glfwSwapInterval(1); // vSync
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.WantCaptureKeyboard = true;
		io.WantCaptureMouse = true;
		ImGui_ImplGlfw_InitForOpenGL(GLFWManager::GetWindow(), false);
		ImGui_ImplOpenGL3_Init("#version 430");
		ImGui::StyleColorsDark();
	}
}


void GLFWManager::Release()
{
	glfwDestroyWindow(window);
	glfwTerminate();
}

int GLFWManager::WindowShouldClose()
{
	return glfwWindowShouldClose(GLFWManager::Instance().window);
}

GLFWwindow* GLFWManager::GetWindow()
{
	return GLFWManager::Instance().window;
}

void GLFWManager::GetCursorPos(double* x, double* y)
{
	glfwGetCursorPos(GLFWManager::Instance().window, x, y);
}

void GLFWManager::SetCursorToPos(double x, double y)
{
	glfwSetCursorPos(GLFWManager::Instance().window, x, y);
}

void GLFWManager::GetWindowSize(int* width, int* height)
{
	glfwGetWindowSize(GLFWManager::Instance().window, width, height);
}

float GLFWManager::GetWindowHeight()
{
	int height, width;
	glfwGetWindowSize(GLFWManager::Instance().window, &width, &height);
	return height;
}

float GLFWManager::GetWindowWidth()
{
	int height, width;
	glfwGetWindowSize(GLFWManager::Instance().window, &width, &height);
	return width;
}

bool GLFWManager::IsKeyDown(int key)
{
	return GLFWManager::Instance().keyState[key];
}

bool GLFWManager::IsMouseDown(int button)
{
	return GLFWManager::Instance().mouseState[button];
}

void GLFWManager::KeyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	ImGuiIO& io = ImGui::GetIO();
	if (action == GLFW_PRESS)
		io.KeysDown[key] = true;
	if (action == GLFW_RELEASE)
		io.KeysDown[key] = false;

	(void)mods;
	io.KeyCtrl = io.KeysDown[GLFW_KEY_LEFT_CONTROL] || io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
	io.KeyShift = io.KeysDown[GLFW_KEY_LEFT_SHIFT] || io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
	io.KeyAlt = io.KeysDown[GLFW_KEY_LEFT_ALT] || io.KeysDown[GLFW_KEY_RIGHT_ALT];
	io.KeySuper = io.KeysDown[GLFW_KEY_LEFT_SUPER] || io.KeysDown[GLFW_KEY_RIGHT_SUPER];

	GLFWManager::Instance().keyState[key] = action == GLFW_PRESS ? true : (action == GLFW_RELEASE ? false : true);
	Renderer* renderer = reinterpret_cast<Renderer*>(glfwGetWindowUserPointer(window));
	renderer->HandleKeyboard(key, scancode, action, mods);
}

void GLFWManager::CharCallback(GLFWwindow* window, unsigned int c)
{
	ImGuiIO& io = ImGui::GetIO();
	if (c > 0 && c < 0x10000)
		io.AddInputCharacter((unsigned short)c);
}

void GLFWManager::ResizeCallback(GLFWwindow* window, int width, int height)
{
	Renderer* renderer = reinterpret_cast<Renderer*>(glfwGetWindowUserPointer(window));
	renderer->HandleResize(width, height);
	glViewport(0, 0, width, height);
}

void GLFWManager::MouseCallback(GLFWwindow * window, int button, int action, int mods)
{
	GLFWManager::Instance().mouseState[button] = action == GLFW_PRESS ? true : (action == GLFW_RELEASE ? false : true);
	Renderer* renderer = reinterpret_cast<Renderer*>(glfwGetWindowUserPointer(window));
	renderer->HandleMouseClick(button, action, mods);
}

void GLFWManager::MousePosCallback(GLFWwindow* window, double xPos, double yPos)
{
	Renderer* renderer = reinterpret_cast<Renderer*>(glfwGetWindowUserPointer(window));
	renderer->HandleMouseMotion(xPos, yPos);
}

void GLFWManager::ErrorCallback(int errorCode, const char* errorDescription)
{
	fprintf(stderr, "Error: %s\n", errorDescription);
}