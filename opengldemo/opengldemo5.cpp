/*#define _CRT_SECURE_NO_WARNINGS
#define GLM_ENABLE_EXPERIMENTAL
#include <iostream>
#include <vector>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
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
#include "shadowmap.hpp"
#include "physicalentity.hpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window, PhysicalEntity& light);

unsigned int SCR_WIDTH = 800;
unsigned int SCR_HEIGHT = 600;

const unsigned int SHADOW_WIDTH = 1024 * 8;
const unsigned int SHADOW_HEIGHT = 1024 * 8;

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

glm::vec3 lightPos(0.0f, 0.0f, 2.0f);

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


	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	GLError(glEnable(GL_DEPTH_TEST));

	Renderer render(cam);

	ShadowMap shadowMap(SHADOW_WIDTH, SHADOW_HEIGHT, 0.01f, 16.5f);
	Shader ourShader3("shadow.glsl", false);
	Shader ourShader("shadowmap.glsl", false);

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

	PhysicalEntity light(meshes, glm::vec3(0.0f, 0.0f, 2.0f));
	light.SetScale(glm::vec3(0.2f));
	PhysicalEntity floor(meshes, glm::vec3(0.0f, -2.0f, 0.0f));
	floor.SetScale(glm::vec3(5.0f));
	PhysicalEntity backpack(backpackModel.m_Meshes);



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

	bool show_another_window = false;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	float lightDif = 0;

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

		processInput(window, light);

		GLError(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));
		GLError(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

		if (!fpsControl)
		{
			cam.SetPitch(camPitch);
			cam.SetYaw(camYaw);
		}
		// 1.
		shadowMap.ConfigureShader(ourShader, light.GetPos());
		shadowMap.BindFrameBuffer();

		floor.Draw(ourShader);
		backpack.Draw(ourShader);

		shadowMap.UnbindFrameBuffer();

		// 2.
		glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::Begin("Opengl Demo");
		ImGui::SliderFloat("Pitch", &camPitch, -180.0f, 180.0f);
		ImGui::SliderFloat("Yaw", &camYaw, -180.0f, 180.0f);
		ImGui::SliderFloat("Light Dif", &lightDif, 0.0f, 1.0f);
		ImGui::End();

		// Rendering
		ImGui::Render();
		int display_w, display_h;
		glfwGetFramebufferSize(window, &display_w, &display_h);

		glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		ourShader3.Bind();
		glm::mat4 projection;
		projection = glm::perspective(glm::radians(45.0f), 16.0f / 9.0f, 0.1f, 1000.0f);
		ourShader3.UniformMatrix4f("uProjection", projection);
		ourShader3.UniformMatrix4f("uView", cam.GetViewMatrix());
		ourShader3.Uniform1v("uViewPos", cam.GetPos());
		ourShader3.UniformLight(glm::vec3(0.3f, 0.3f, 0.3f), glm::vec3(lightDif), glm::vec3(1.0f, 1.0f, 1.0f), light.GetPos());
		ourShader3.UniformMatrix4f("uLightSpaceMatrix", shadowMap.GetLightSpaceMatrix());

		shadowMap.BindTexture(5);
		ourShader3.Uniform1i("uDepthMap", 5);

		floor.Draw(ourShader3);
		light.Draw(ourShader3);
		backpack.Draw(ourShader3);
		frameCount++;

		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}

void processInput(GLFWwindow* window, PhysicalEntity& light)
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
		light.SetPos(light.GetPos() - glm::vec3(0.0f, 0.0f, 0.05f));
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		light.SetPos(light.GetPos() + glm::vec3(0.0f, 0.0f, 0.05f));
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		light.SetPos(light.GetPos() + glm::vec3(0.05f, 0.0f, 0.0f));
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		light.SetPos(light.GetPos() - glm::vec3(0.05f, 0.0f, 0.0f));
	if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS)
		light.SetPos(light.GetPos() + glm::vec3(0.0f, 0.05f, 0.0f));
	if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS)
		light.SetPos(light.GetPos() - glm::vec3(0.0f, 0.05f, 0.0f));
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
}*/