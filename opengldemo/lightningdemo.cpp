#define _CRT_SECURE_NO_WARNINGS

#define GLM_ENABLE_EXPERIMENTAL
#include <iostream>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/ext.hpp>
#include "shader.hpp"
#include "vertex.hpp"
#include "texture.hpp"
#include "camera.hpp"
#include "renderer.hpp"
#include "physical.hpp"
#include "vertex_data.hpp"
#include "global.hpp"
#include "bullet.hpp"
#include "material.hpp"
#include "Light.hpp"


void framebuffer_size_callback1(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window, Entity& lightCube);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

float lastX1 = SCR_WIDTH / 2.0f;
float lastY1 = SCR_HEIGHT / 2.0f;
bool firstMouse1 = true;

float deltaTime1 = 0.0f;
float lastFrame1 = 0.0f;
static int frameCount = 0;

static Camera cam(glm::vec3(0.0f, 0.0f, 5.0f));

float ambientStrength = 0.1f;

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback1);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glEnable(GL_DEPTH_TEST);

	Shader lightingShader("1.colors.glsl");
	Shader lightCubeShader("1.light_cube.glsl");

	unsigned int indices[] = {
		0
	};

	VertexArray cubeVA(boxVertices, indices, 36, 8 * sizeof(float), 0);

	Renderer renderer(cam);
	Texture texture("container2.png");
	Texture texture2("container2_specular.png");

	Material containerMaterial = { texture, texture2, 32.0f };

	Physical cube(glm::vec3(0.0f), glm::vec3(0.0f), lightingShader, cubeVA, texture, containerMaterial);
	Physical lightCube(glm::vec3(1.0f, 0.0f, 1.0f), glm::vec3(0.0f), lightCubeShader, cubeVA, texture, containerMaterial);

	lightCube.SetScale(glm::vec3(0.2));

	Light light = { glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f)};

	lightingShader.Bind();

	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime1 = currentFrame - lastFrame1;
		lastFrame1 = currentFrame;

		processInput(window, lightCube);

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		lightingShader.Bind();

		std::vector<float> lightCubeColors;
		std::vector<float> lightCubePos;

		lightCubeColors.push_back(light.Color.r);
		lightCubeColors.push_back(light.Color.g);
		lightCubeColors.push_back(light.Color.b);

		lightCubePos.push_back(lightCube.GetPos().x);
		lightCubePos.push_back(lightCube.GetPos().y);
		lightCubePos.push_back(lightCube.GetPos().z);

		lightingShader.Uniform3fv("uLightPos", 1, lightCubePos.data());
		lightingShader.Uniform3fv("uLightColors", 1, lightCubeColors.data());
		lightingShader.Uniform3f("uCamPos", cam.GetPos().x, cam.GetPos().y, cam.GetPos().z);

		lightingShader.UniformLight(light);

		renderer.Render(cube);
		renderer.Render(lightCube);

		frameCount++;

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

void processInput(GLFWwindow* window, Entity& lightCube)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cam.MoveForward(deltaTime1 * 2.0f);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cam.MoveBackward(deltaTime1 * 2.0f);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cam.MoveLeft(deltaTime1 * 2.0f);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cam.MoveRight(deltaTime1 * 2.0f);
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
		ambientStrength += 0.01f;
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
		ambientStrength -= 0.01f;
	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
		lightCube.Move(glm::vec3(0.0f, 0.0f, -0.01f));
	if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS)
		lightCube.Move(glm::vec3(-0.01f, 0.0f, 0.0f));
	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
		lightCube.Move(glm::vec3(0.0f, 0.0f, 0.01f));
	if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
		lightCube.Move(glm::vec3(0.01f, 0.0f, 0.0f));
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
		lightCube.Move(glm::vec3(0.0f, 0.01f, 0.0f));
	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
		lightCube.Move(glm::vec3(0.0f, -0.01f, 0.0f));
}

void framebuffer_size_callback1(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	if (firstMouse1)
	{
		lastX1 = xpos;
		lastY1 = ypos;
		firstMouse1 = false;
	}

	float xoffset = xpos - lastX1;
	float yoffset = lastY1 - ypos;

	lastX1 = xpos;
	lastY1 = ypos;

	cam.RotateHorizontally(xoffset / 5.0f);
	cam.RotateVertically(yoffset / 5.0f);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
}