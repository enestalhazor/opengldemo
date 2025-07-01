#define _CRT_SECURE_NO_WARNINGS

#define GLM_ENABLE_EXPERIMENTAL
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/ext.hpp>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
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
#include "cubemap.hpp"
#include "physicalentity.hpp"
#include "light.hpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
void ImGuiProcesses(GLFWwindow* window);
template <typename T>
void ImguiRender(std::vector<T>& entities);



const unsigned int SHADOW_WIDTH = 1024 * 2;
const unsigned int SHADOW_HEIGHT = 1024 * 2;
unsigned int SCR_WIDTH = 800;
unsigned int SCR_HEIGHT = 600;

bool firstMouse1 = true;
bool fpsControl = true;

float lastX1 = SCR_WIDTH / 2.0f;
float lastY1 = SCR_HEIGHT / 2.0f;
float deltaTime1 = 0.0f;
float lastFrame1 = 0.0f;
float camPitch = 0;
float camYaw = 0;

static int frameCount = 0;
static Camera cam(glm::vec3(0.0f, 0.0f, 3.0f));

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

	//  Renderer renderer(cam);

	Shader ourShader3("pointshadow.glsl", false);
	Shader lightShader("lightshader.glsl", false);
	Shader cubeMapShader("pointshadow_depth.glsl", true);

	Texture floorDiffuse("wood.png", "mytextures", "texture_diffuse");
	Texture floorSpecular("wood.png", "mytextures", "texture_specular");

	std::vector<Texture> textures;
	std::vector<Vertex> vertices;
	std::unordered_map<std::string, std::shared_ptr<PhysicalEntity>> entities;
	std::vector<Light> lights;
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
	Model backpackModel("backpack/backpack.obj");

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

	std::vector<Mesh> meshes;
	Mesh mesh(vertices, indices, textures);
	meshes.push_back(mesh);

	entities["floor"] = std::make_shared<PhysicalEntity>(meshes, glm::vec3(0.0f, -2.0f, 0.0f));
	entities["backpack1"] = std::make_shared<PhysicalEntity>(backpackModel.m_Meshes);
	entities["backpack2"] = std::make_shared<PhysicalEntity>(backpackModel.m_Meshes);
	entities["mini_backpack"] = std::make_shared<PhysicalEntity>(backpackModel.m_Meshes);

	entities["floor"]->SetScale(glm::vec3(5.0f));
	entities["backpack2"]->SetScale(glm::vec3(0.5f));
	entities["backpack2"]->SetPos(glm::vec3(3.0f, 0.0f, 0.0f));
	entities["backpack2"]->SetSpeed(glm::vec3(0.01f, 0.0f, 0.06f));
	entities["mini_backpack"]->SetScale(glm::vec3(0.01f));
	entities["mini_backpack"]->SetPos(glm::vec3(0.0f, 0.0f, 1.5f));

	lights.emplace_back(glm::vec3(0.3f), glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(0.0f, 0.0f, 2.0f), meshes);
	lights.emplace_back(glm::vec3(0.3f), glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(0.0f, 3.0f, 2.0f), meshes);

	for (int i = 0; i < 3; i++)
	{
		lights.emplace_back(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 0.0f), meshes);
	}

	// imgui
	ImGuiProcesses(window);
	// imgui

	for (auto& light : lights)
	{
		light.SetScale(glm::vec3(0.2f));
	}

	Renderer renderer(cam, ourShader3);

	while (!glfwWindowShouldClose(window))
	{
		if (fpsControl)
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}
		else
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}

		glfwPollEvents();

		if (glfwGetWindowAttrib(window, GLFW_ICONIFIED) != 0)
		{
			ImGui_ImplGlfw_Sleep(10);
			continue;
		}

		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime1 = currentFrame - lastFrame1;
		lastFrame1 = currentFrame;

		processInput(window);

		GLError(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));

		if (!fpsControl)
		{
			cam.SetPitch(camPitch);
			cam.SetYaw(camYaw);
		}

		// 1.
		for (int i = 0; i < lights.size(); i++)
		{
			lights[i].GetCubeMap().ConfigureShader(cubeMapShader, lights[i].GetPos());
			lights[i].GetCubeMap().BindFrameBuffer();

			for (auto& pair : entities) {
				pair.second->Draw(cubeMapShader);
			}

			lights[i].GetCubeMap().UnbindFrameBuffer();
		}

		// 2.
		glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
		ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

		glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		renderer.Draw(lights, entities);
		ImguiRender(lights);

		entities["mini_backpack"]->RotateHorizontally(5.0f);
		entities["backpack2"]->Move();
		entities["floor"]->RotateHorizontally(1.0f);
		frameCount++;

		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}

template <typename T>
void ImguiRender(std::vector<T>& entities)
{
	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin("OpenglDemo Enes");
	ImGui::SliderFloat("Pitch", &camPitch, -180.0f, 180.0f);
	ImGui::SliderFloat("Yaw", &camYaw, -180.0f, 180.0f);
	for (int i = 0; i < entities.size(); i++)
	{
		ImGui::SliderFloat3((std::to_string(i + 1) + ". Light pos").c_str(), (&entities[i].GetPos().z, &entities[i].GetPos().y, &entities[i].GetPos().x), -10.0f, 10.0f);
	}

	ImGui::End();

	// Rendering
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ImGuiProcesses(GLFWwindow* window)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 130");
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
	if (glfwGetKey(window, GLFW_KEY_F6) == GLFW_PRESS)
		fpsControl = !fpsControl;
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
	if (!fpsControl)
	{
		return;
	}

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

	camPitch = cam.GetPitch();
	camYaw = cam.GetYaw();
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
}