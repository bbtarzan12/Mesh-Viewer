#pragma once
#include "../Core/SingletonManager.h"

struct GLFWwindow;

class GLFWManager : public SingletonManager<GLFWManager>
{
	friend class SingletonManager<GLFWManager>;

public:
	void Init(int width, int height, const char* name, void* renderer);
	void Release();

	// GLFW APIs
	static int WindowShouldClose();
	static GLFWwindow* GetWindow();
	static void GetCursorPos(double* x, double* y);
	static void SetCursorToPos(double x, double y);
	static void GetWindowSize(int* width, int* height);
	static float GetWindowHeight();
	static float GetWindowWidth();

	// Inputs
	static bool IsKeyDown(int key);
	static bool IsMouseDown(int button);

	// Callbacks
	static void KeyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void CharCallback(GLFWwindow* window, unsigned int c);
	static void ResizeCallback(GLFWwindow* window, int width, int height);
	static void MouseCallback(GLFWwindow* window, int button, int action, int mods);
	static void MousePosCallback(GLFWwindow* window, double xPos, double yPos);
	static void ErrorCallback(int errorCode, const char* errorDescription);


private:
	GLFWManager() = default;
	virtual ~GLFWManager() = default;

private:
	GLFWwindow * window;
	bool keyState[256] = { false };
	bool mouseState[3] = { false };
};