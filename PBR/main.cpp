#include "Core/Camera.h"
#include "Core/Renderer.h"

//int main(void)
//{
//	if (!glfwInit())
//	{
//		fprintf(stderr, "Failed to initialize GLFW\n");
//		getchar();
//		return -1;
//	}
//
//	glfwWindowHint(GLFW_SAMPLES, 4);
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
//	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//
//	int width = 1024;
//	int height = 720;
//
//	// Open a window and create its OpenGL context
//	GLFWwindow* window = glfwCreateWindow(width, height, "PBR", NULL, NULL);
//	if (window == NULL)
//	{
//		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible.\n");
//		getchar();
//		glfwTerminate();
//		return -1;
//	}
//	glfwMakeContextCurrent(window);
//
//	// Initialize GLEW
//	if (glewInit() != GLEW_OK)
//	{
//		fprintf(stderr, "Failed to initialize GLEW\n");
//		getchar();
//		glfwTerminate();
//		return -1;
//	}
//
//	glEnable(GL_DEPTH_TEST);
//	glDepthFunc(GL_LESS);
//	glEnable(GL_CULL_FACE);
//
//	// Ensure we can capture the escape key being pressed below
//	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
//
//	// Dark blue background
//	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
//
//	std::vector<GLfloat> vertices, normals, uvs;
//	MeshLoader::Load("Meshes/basket_ball.obj", vertices, normals, uvs);
//
//	GLuint vertexArrayID;
//	glGenVertexArrays(1, &vertexArrayID);
//	glBindVertexArray(vertexArrayID);
//
//	GLuint vertexPositionID;
//	glGenBuffers(1, &vertexPositionID);
//	glBindBuffer(GL_ARRAY_BUFFER, vertexPositionID);
//	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
//
//	GLuint vertexNormalID;
//	glGenBuffers(1, &vertexNormalID);
//	glBindBuffer(GL_ARRAY_BUFFER, vertexNormalID);
//	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(float), normals.data(), GL_STATIC_DRAW);
//
//	GLuint vertexUVID;
//	glGenBuffers(1, &vertexUVID);
//	glBindBuffer(GL_ARRAY_BUFFER, vertexUVID);
//	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(float), uvs.data(), GL_STATIC_DRAW);
//
//	GLuint textureID = TextureLoader::Load("Images/basket_ball_diffuse.png");
//
//	GLuint programID = ShaderLoader::Load("Shaders/phong_vert.glsl", "Shaders/phong_frag.glsl");
//
//	Camera camera({ width, height }, 45.0f, { 0.1f, 1000.0f });
//
//	glm::mat4 projectionMatrix = glm::perspective(glm::radians(45.0f), width / (float)height, 0.1f, 100.0f);
//	glm::mat4 viewMatrix = glm::lookAt(glm::vec3(30, 1, 30), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
//	glm::mat4 modelMatrix(1.0f);
//	glm::mat4 mvpMatrix = projectionMatrix * viewMatrix * modelMatrix;
//	
//	GLuint mvpMatrixID = glGetUniformLocation(programID, "MVP");
//	GLuint viewMatrixID = glGetUniformLocation(programID, "V");
//	GLuint modelMatrixID = glGetUniformLocation(programID, "M");
//	GLuint lightPositionID = glGetUniformLocation(programID, "LightPosition_worldspace");
//
//	float lastTime = glfwGetTime();
//	float deltaTime = 0;
//	do
//	{
//		// Clear the screen. It's not mentioned before Tutorial 02, but it can cause flickering, so it's there nonetheless.
//		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//		double currentTime = glfwGetTime();
//		deltaTime = currentTime - lastTime;
//		lastTime = currentTime;
//
//		glUseProgram(programID);
//		glUniformMatrix4fv(mvpMatrixID, 1, GL_FALSE, &mvpMatrix[0][0]);
//		glUniformMatrix4fv(viewMatrixID, 1, GL_FALSE, &viewMatrix[0][0]);
//		glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, &modelMatrix[0][0]);
//		glUniform3f(lightPositionID, -30.0f, 3.0f, 30.0f);
//
//
//		glBindTexture(GL_TEXTURE_2D, textureID);
//
//		glEnableVertexAttribArray(0);
//		glBindBuffer(GL_ARRAY_BUFFER, vertexPositionID);
//		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
//
//		glEnableVertexAttribArray(1);
//		glBindBuffer(GL_ARRAY_BUFFER, vertexNormalID);
//		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
//
//		glEnableVertexAttribArray(2);
//		glBindBuffer(GL_ARRAY_BUFFER, vertexUVID);
//		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
//
//		glDrawArrays(GL_TRIANGLES, 0, vertices.size() * 3);
//		glDisableVertexAttribArray(0);
//		glUseProgram(0);
//
//		// Swap buffers
//		glfwSwapBuffers(window);
//		glfwPollEvents();
//
//	} // Check if the ESC key was pressed or the window was closed
//	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&	glfwWindowShouldClose(window) == 0);
//
//	glfwDestroyWindow(window);
//
//	// Close OpenGL window and terminate GLFW
//	glfwTerminate();
//
//	return 0;
//}

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