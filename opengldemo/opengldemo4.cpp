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
#include "ui.hpp"

Renderer* rendererPtr;
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window, std::vector<Light>& lights, std::shared_ptr<PhysicalEntity> flash);
void ImGuiProcesses(GLFWwindow* window);
void frameBufferSizeCallback(GLFWwindow* window, int w, int h);
template <typename T>
void ImguiRender(std::vector<T>& entities);

const unsigned int SHADOW_WIDTH = 1024 * 2;
const unsigned int SHADOW_HEIGHT = 1024 * 2;

bool firstMouse1 = true;
bool fpsControl = true;
bool gunAnimation = false;

float lastX1 = 0.0f;
float lastY1 = 0.0f;
float deltaTime1 = 0.0f;
float lastFrame1 = 0.0f;
float camPitch = 0;
float camYaw = 0;
float gunPitch = 0;

static Camera cam(glm::vec3(0.0f, 2.0f, 3.0f));

glm::vec3 lightDiffuse(1.0f, 1.0f, 1.0f);


int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	GLError(glEnable(GL_DEPTH_TEST));
	GLError(glEnable(GL_BLEND));
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//  Renderer renderer(cam);

	Shader ourShader3("pointshadow.glsl", false);
	Shader lightShader("lightshader.glsl", false);
	Shader cubeMapShader("pointshadow_depth.glsl", true);

	Texture muzzleFlash("muzzleflash.png", "mytextures", "texture_diffuse");
	Texture cross("cross.png", "mytextures", "texture_diffuse");
	Texture floorDiffuse("wood.png", "mytextures", "texture_diffuse");
	Texture floorSpecular("wood.png", "mytextures", "texture_specular");

	std::vector<Texture> textures;
	textures.emplace_back(floorDiffuse);
	textures.emplace_back(floorSpecular);

	std::vector<Texture> textures2;
	textures2.emplace_back(muzzleFlash);

	std::vector<Texture> textures3;
	textures3.emplace_back(cross);

	std::vector<Vertex> vertices;
	std::vector<Vertex> vertices2;
	std::unordered_map<std::string, std::shared_ptr<PhysicalEntity>> entities;
	std::unordered_map<std::string, std::shared_ptr<UI>> UIs;
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

	Model backpackModel("backpack/backpack.obj_model");
	Model place("models/forest/forest.obj_model");
	Model guy("models/guy/bearded_guy_idle_anim.obj_model");
	Model gun("models/gun/pistol.obj_model");

	for (int i = 0; i < 4; i++)
	{
		vertices.emplace_back(ground[i]);
		vertices2.emplace_back(v[i]);
	}

	for (int i = 0; i < 6; i++)
	{
		indices.emplace_back(d_indices[i]);
	}

	Mesh mesh(vertices, indices, textures);
	Mesh mFlash(vertices, indices, textures2);
	Mesh mCross(vertices2, indices, textures3);

	std::vector<Mesh> meshes;
	meshes.push_back(mesh);

	std::vector<Mesh> meshes2;
	meshes2.push_back(mFlash);

	// UI
	UIs["crosshair"] = UI::NewBasic(v, d_indices, cross, "cross.glsl");

	// PhysicalEntity
	entities["backpack1"] = PhysicalEntity::NewFromModel(backpackModel);
	entities["backpack1"]->SetPos(glm::vec3(1.10f, 0.14f, 1.15f));
	entities["backpack1"]->SetScale(glm::vec3(0.01f));
	entities["muzzleflash"] = PhysicalEntity::NewBasic(v, d_indices, muzzleFlash);
	entities["muzzleflash"]->SetScale(glm::vec3(0.0f));
	entities["muzzleflash"]->SetPitch(90.0f);
	entities["place"] = std::make_shared<PhysicalEntity>(place.m_Meshes);
	entities["place"]->SetScale(glm::vec3(0.2f));
	entities["guy"] = std::make_shared<PhysicalEntity>(guy.m_Meshes);
	entities["guy"]->SetPos(glm::vec3(1.10f, 0.09f, 1.2f));
	entities["guy"]->SetScale(glm::vec3(0.05f));
	entities["guy"]->RotateVertically(90.0f);
	entities["gun"] = std::make_shared<PhysicalEntity>(gun.m_Meshes);
	entities["gun"]->SetScale(glm::vec3(0.0005f));

	lights.emplace_back(glm::vec3(0.3f), glm::vec3(0.5f), glm::vec3(0.5f), glm::vec3(0.0f, 1.0f, 2.0f), meshes);
	lights.emplace_back(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.0f), cam.GetPos() + cam.GetDirection() * 0.1f, meshes);

	for (int i = 0; i < 3; i++)
	{
		lights.emplace_back(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(100.0f, 0.0f, 0.0f), meshes);
	}
	ImGuiProcesses(window);

	for (auto& light : lights)
	{
		light.SetScale(glm::vec3(0.2f));
	}

	Renderer renderer(cam, ourShader3);

	glfwSetFramebufferSizeCallback(window, frameBufferSizeCallback);

	rendererPtr = &renderer;

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

		if (!fpsControl)
		{
			cam.SetPitch(camPitch);
			cam.SetYaw(camYaw);
		}

		switch (gunAnimation)
		{
		case true:
			gunPitch += 3.0f;

			entities["muzzleflash"]->SetScale(glm::vec3(0.0f));
			lights[1].SetAmbient(glm::vec3(0.0f));
			lights[1].SetDiffuse(glm::vec3(0.0f));
			lights[1].SetSpecular(glm::vec3(0.0f));

			if (gunPitch >= 15.0f)
			{
				gunAnimation = false;
				gunPitch = 0.0f;

			}
			break;
		}

		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime1 = currentFrame - lastFrame1;
		lastFrame1 = currentFrame;

		processInput(window, lights, entities["muzzleflash"]);

		entities["gun"]->SetPos(cam.GetPos() + (cam.GetDirection() + glm::vec3(0.0f, -0.4f, 0.0f) + glm::cross(cam.GetDirection(), glm::vec3(0.0f, 1.0f, 0.0f)) * 0.5f) * 0.02f);
		entities["gun"]->SetYaw(-cam.GetYaw() + 90.0f);
		entities["gun"]->SetPitch(-cam.GetPitch() - gunPitch);
		lights[1].SetPos(cam.GetPos() + cam.GetDirection() * 0.05f);
		entities["muzzleflash"]->SetPos(entities["gun"]->GetPos() + (cam.GetDirection() * 0.01f) + glm::vec3(0.0f, 0.003f, 0.0f));
		entities["muzzleflash"]->SetYaw(-cam.GetYaw() + 90.0f);
		entities["muzzleflash"]->SetPitch(-cam.GetPitch() + 90.0f);

		renderer.Render(lights, entities, cubeMapShader);
		ImguiRender(lights);
		// 
		renderer.RenderUI(UIs);

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

void frameBufferSizeCallback(GLFWwindow* window, int w, int h)
{
	rendererPtr->SetScreenH(h);
	rendererPtr->SetScreenW(w);
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

void processInput(GLFWwindow* window, std::vector<Light>& lights, std::shared_ptr<PhysicalEntity> flash)
{
	float camSpeed = 0.5f;
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cam.MoveForward(deltaTime1 * camSpeed);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cam.MoveLeft(deltaTime1 * camSpeed);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cam.MoveBackward(deltaTime1 * camSpeed);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cam.MoveRight(deltaTime1 * camSpeed);
	if (glfwGetKey(window, GLFW_KEY_F6) == GLFW_PRESS)
		fpsControl = !fpsControl;
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		if (!gunAnimation)
		{
			lights[1].SetAmbient(glm::vec3(0.1f));
			lights[1].SetDiffuse(glm::vec3(0.3f));
			lights[1].SetSpecular(glm::vec3(1.0f));
			flash->SetScale(glm::vec3(0.003f));

			gunAnimation = true;
		}
	}
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