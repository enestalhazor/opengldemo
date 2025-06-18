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
#include "vertex_array.hpp"
#include "texture.hpp"
#include "camera.hpp"
#include "renderer.hpp"
#include "vertex_data.hpp"
#include "global.hpp"
#include "material.hpp"
#include "mesh.hpp"
#include "model.hpp"
#include "glerror.hpp"

void pushTransforms(glm::vec3 lightPos, glm::mat4 shadowProjection, std::vector<glm::mat4>& shadowTransforms, glm::vec3 v1, glm::vec3 v2);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

unsigned int SCR_WIDTH = 800;
unsigned int SCR_HEIGHT = 600;
unsigned int CubeMap;

const unsigned int SHADOW_WIDTH = 1024 * 4;
const unsigned int SHADOW_HEIGHT = 1024 * 4;

float lastX1 = SCR_WIDTH / 2.0f;
float lastY1 = SCR_HEIGHT / 2.0f;
bool firstMouse1 = true;
bool blinn = false;
bool blinnKeyPressed = false;

float deltaTime1 = 0.0f;
float lastFrame1 = 0.0f;
static int frameCount = 0;
int p = 0;

float near_plane = 0.1f;
float far_plane = 25.0f;

static Camera cam(glm::vec3(0.0f, 0.0f, 3.0f));

glm::vec3 lightPos(0.0f, 0.0f, 2.0f);
glm::vec3 lightDiffuse(1.0f, 1.0f, 1.0f);


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
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	GLError(glEnable(GL_DEPTH_TEST));

	Renderer render(cam);

	Shader ourShader3("pointshadow.glsl", false);
	Shader lightShader("lightshader.glsl", false);
	Shader cubeMapShader("pointshadow_depth.glsl", true);

	Texture cubeMap(SHADOW_WIDTH, SHADOW_HEIGHT, CubeMap);
	Texture floorDiffuse("wood.png", "mytextures", "texture_diffuse");
	Texture floorSpecular("wood.png", "mytextures", "texture_specular");

	std::vector<Texture> textures;
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;


	unsigned int d_indices[] = {
			0, 1, 3,
			1, 2, 3
	};

	Vertex v[] = { {glm::vec3(1.0f, -1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)},
					{glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)},
					{glm::vec3(-1.0f, 1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)},
					{glm::vec3(-1.0f, -1.0f, 0.0f),glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)} };

	Vertex ground[] = { {glm::vec3(1.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)},
					{glm::vec3(1.0f, 0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)},
					{glm::vec3(-1.0f, 0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)},
					{glm::vec3(-1.0f, 0.0f, -1.0f),glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)} };

	VertexArray vA(v, d_indices, 4, sizeof(Vertex), 6);
	Model backpack("backpack/backpack.obj");

	textures.emplace_back(floorDiffuse);
	textures.emplace_back(floorSpecular);

	for (int i = 0; i < 4; i++)
	{
		vertices.emplace_back(ground[i]);
	}

	for (int i = 0; i < 6; i++)
	{
		indices.emplace_back(d_indices[i]);
	}

	Mesh mesh(vertices, indices, textures);

	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime1 = currentFrame - lastFrame1;
		lastFrame1 = currentFrame;

		processInput(window);

		GLError(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));
		GLError(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));


		glm::mat4 shadowProjection = glm::perspective(glm::radians(90.0f), (float)SHADOW_WIDTH / (float)SHADOW_HEIGHT, near_plane, far_plane);

		std::vector<glm::mat4> shadowTransforms;

		pushTransforms(lightPos, shadowProjection, shadowTransforms, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
		pushTransforms(lightPos, shadowProjection, shadowTransforms, glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
		pushTransforms(lightPos, shadowProjection, shadowTransforms, glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		pushTransforms(lightPos, shadowProjection, shadowTransforms, glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f));
		pushTransforms(lightPos, shadowProjection, shadowTransforms, glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f));
		pushTransforms(lightPos, shadowProjection, shadowTransforms, glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f));

		glm::mat4 model = glm::mat4(1.0f);

		// 1.
		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, cubeMap.GetFBO());
		glClear(GL_DEPTH_BUFFER_BIT);
		cubeMapShader.Bind();

		for (unsigned int i = 0; i < 6; ++i)
		{
			cubeMapShader.UniformMatrix4f("shadowMatrices[" + std::to_string(i) + "]", shadowTransforms[i]);
		}

		cubeMapShader.Uniform1f("far_plane", far_plane);
		cubeMapShader.Uniform1v("lightPos", lightPos);

		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		cubeMapShader.UniformMatrix4f("uModel", model);
		mesh.Draw(cubeMapShader, 1);

		model = glm::mat4(1.0f);
		model = glm::translate(model, lightPos + glm::vec3(0.0f, -0.2f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		cubeMapShader.UniformMatrix4f("uModel", model);
		mesh.Draw(cubeMapShader, 1);

		model = glm::mat4(1.0f);
		cubeMapShader.UniformMatrix4f("uModel", model);
		backpack.Draw(cubeMapShader, 1);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// 2.
		glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ourShader3.Bind();
		glm::mat4 projection;
		projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / float(SCR_HEIGHT), 0.1f, 100.0f);
		ourShader3.UniformMatrix4f("uProjection", projection);
		ourShader3.UniformMatrix4f("uView", cam.GetViewMatrix());
		ourShader3.Uniform1v("viewPos", cam.GetPos());
		ourShader3.UniformLight(glm::vec3(0.3f, 0.3f, 0.3f), lightDiffuse, glm::vec3(1.0f, 1.0f, 1.0f), lightPos);
		ourShader3.Uniform1f("far_plane", far_plane);

		cubeMap.BindCubeMap(5, CubeMap);
		ourShader3.Uniform1i("depthMap", 5);

		// floor
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		ourShader3.UniformMatrix4f("uModel", model);
		mesh.Draw(ourShader3, 0);

		model = glm::mat4(1.0f);
		model = glm::translate(model, lightPos + glm::vec3(0.0f, -0.2f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		ourShader3.UniformMatrix4f("uModel", model);
		mesh.Draw(ourShader3, 0);

		// light
		model = glm::mat4(1.0f);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		lightShader.UniformMatrix4f("uModel", model);
		lightShader.UniformMatrix4f("uProjection", projection);
		lightShader.UniformMatrix4f("uView", cam.GetViewMatrix());
		mesh.Draw(lightShader, 0);

		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		lightShader.UniformMatrix4f("uModel", model);
		mesh.Draw(lightShader, 0);

		// backpack
		model = glm::mat4(1.0f);
		ourShader3.UniformMatrix4f("uModel", model);
		backpack.Draw(ourShader3, 0);

		frameCount++;

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

void pushTransforms(glm::vec3 lightPos, glm::mat4 shadowProjection, std::vector<glm::mat4>& shadowTransforms, glm::vec3 v1, glm::vec3 v2)
{
	shadowTransforms.push_back(shadowProjection * glm::lookAt(lightPos, lightPos + v1, v2));
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cam.MoveForward(deltaTime1 * 2.0f);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cam.MoveLeft(deltaTime1 * 2.0f);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cam.MoveBackward(deltaTime1 * 2.0f);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cam.MoveRight(deltaTime1 * 2.0f);

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		lightPos.z -= 0.05f;
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		lightPos.z += 0.05f;
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		lightPos.x += 0.05f;
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		lightPos.x -= 0.05f;
	if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS)
		lightPos.y += 0.05f;
	if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS)
		lightPos.y -= 0.05f;

	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		lightDiffuse.r += 0.01f;
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
		lightDiffuse.g += 0.01f;
	if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
		lightDiffuse.b += 0.01f;

	if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
		lightDiffuse.r -= 0.01f;
	if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
		lightDiffuse.g -= 0.01f;
	if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS)
		lightDiffuse.b -= 0.01f;

	if (lightDiffuse.r > 1.0f)
	{
		lightDiffuse.r = 1.0f;
	}
	if (lightDiffuse.r < 0.0f)
	{
		lightDiffuse.r = 0.0f;
	}


	if (lightDiffuse.g > 1.0f)
	{
		lightDiffuse.g = 1.0f;
	}
	if (lightDiffuse.g < 0.0f)
	{
		lightDiffuse.g = 0.0f;
	}


	if (lightDiffuse.b > 1.0f)
	{
		lightDiffuse.b = 1.0f;
	}
	if (lightDiffuse.b < 0.0f)
	{
		lightDiffuse.b = 0.0f;
	}


}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	std::cout << "width " << width << "height " << height << std::endl;
	SCR_WIDTH = width;
	SCR_HEIGHT = height;
	GLError(glViewport(0, 0, width, height));
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