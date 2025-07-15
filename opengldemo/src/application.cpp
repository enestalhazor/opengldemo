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
#include "mesh.hpp"
#include "model.hpp"
#include "glerror.hpp"
#include "cubemap.hpp"
#include "physicalentity.hpp"
#include "light.hpp"
#include "uientity.hpp"
#include "textentity.hpp"

Renderer* rendererPtr;
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window, std::vector<Light>& lights, std::shared_ptr<PhysicalEntity> flash, unsigned int& ammoNum);
void ImGuiProcesses(GLFWwindow* window);
void frameBufferSizeCallback(GLFWwindow* window, int w, int h);
template <typename T>
void ImguiRender(std::vector<T>& entities);
void RenderText(std::unordered_map<std::string, std::shared_ptr<TextEntity>> texts, std::shared_ptr<UIEntity> singleCharEntity);

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
std::unordered_map<char, Vertex*> vertexMap;
glm::vec3 lightDiffuse(1.0f, 1.0f, 1.0f);

float char_w = 1.0f / 5.0f;
float char_h = 1.0f / 9.0f;


Vertex planeVertices[] = { {glm::vec3(1.0f, -1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)},
					{glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)},
					{glm::vec3(-1.0f, 1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)},
					{glm::vec3(-1.0f, -1.0f, 0.0f),glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)} };

unsigned int planeIndices[] = {
			0, 1, 3,
			1, 2, 3
};

// bottom right 1.
// top right 2.
// top left 3.
// bottom left 4.

Vertex oneVa[] = { {glm::vec3(0.25f, -0.25f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(char_w , 1 - char_h), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)},
				{glm::vec3(0.25f, 0.25f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(char_w, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)},
				{glm::vec3(-0.25f, 0.25f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)},
				{glm::vec3(-0.25f, -0.25f, 0.0f),glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 1 - char_h), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)} };

Vertex twoVa[] = { {glm::vec3(0.25f, -0.25f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(char_w + char_w , 1 - char_h), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)},
				{glm::vec3(0.25f, 0.25f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(char_w + char_w, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)},
				{glm::vec3(-0.25f, 0.25f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f + char_w, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)},
				{glm::vec3(-0.25f, -0.25f, 0.0f),glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f + char_w, 1 - char_h), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)} };

Vertex threeVa[] = { {glm::vec3(0.25f, -0.25f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(char_w + char_w * 2, 1 - char_h), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)},
				{glm::vec3(0.25f, 0.25f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(char_w + char_w * 2, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)},
				{glm::vec3(-0.25f, 0.25f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f + char_w * 2, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)},
				{glm::vec3(-0.25f, -0.25f, 0.0f),glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f + char_w * 2, 1 - char_h), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)} };

Vertex fourVa[] = { {glm::vec3(0.25f, -0.25f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(char_w + char_w * 3, 1 - char_h), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)},
				{glm::vec3(0.25f, 0.25f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(char_w + char_w * 3, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)},
				{glm::vec3(-0.25f, 0.25f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f + (char_w - 0.005) * 3, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)},
				{glm::vec3(-0.25f, -0.25f, 0.0f),glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f + (char_w - 0.005) * 3, 1 - char_h), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)} };

Vertex fiveVa[] = { {glm::vec3(0.25f, -0.25f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 0.89f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)},
				{glm::vec3(0.25f, 0.25f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)},
				{glm::vec3(-0.25f, 0.25f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.8f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)},
				{glm::vec3(-0.25f, -0.25f, 0.0f),glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.8f, 0.89f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)} };

Vertex sixVa[] = { {glm::vec3(0.25f, -0.25f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(char_w , 0.89f - char_h), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)},
				{glm::vec3(0.25f, 0.25f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(char_w, 0.89f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)},
				{glm::vec3(-0.25f, 0.25f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.89f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)},
				{glm::vec3(-0.25f, -0.25f, 0.0f),glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.89f - char_h), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)} };

Vertex sevenVa[] = { {glm::vec3(0.25f, -0.25f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(char_w + char_w , 0.89f - char_h), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)},
				{glm::vec3(0.25f, 0.25f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(char_w + char_w, 0.89f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)},
				{glm::vec3(-0.25f, 0.25f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f + char_w, 0.89f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)},
				{glm::vec3(-0.25f, -0.25f, 0.0f),glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f + char_w, 0.89f - char_h), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)} };

Vertex eightVa[] = { {glm::vec3(0.25f, -0.25f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(char_w + char_w * 2, 0.89 - char_h), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)},
				{glm::vec3(0.25f, 0.25f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(char_w + char_w * 2, 0.89f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)},
				{glm::vec3(-0.25f, 0.25f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f + char_w * 2, 0.89f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)},
				{glm::vec3(-0.25f, -0.25f, 0.0f),glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f + char_w * 2, 0.89f - char_h), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)} };

Vertex nineVa[] = { {glm::vec3(0.25f, -0.25f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(char_w + char_w * 3, 0.89f - char_h), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)},
				{glm::vec3(0.25f, 0.25f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(char_w + char_w * 3, 0.89f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)},
				{glm::vec3(-0.25f, 0.25f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f + char_w * 3, 0.89f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)},
				{glm::vec3(-0.25f, -0.25f, 0.0f),glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f + char_w * 3, 0.89f - char_h), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)} };

Vertex zeroVa[] = { {glm::vec3(0.25f, -0.25f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 0.885f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)},
				{glm::vec3(0.25f, 0.25f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 0.773f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)},
				{glm::vec3(-0.25f, 0.25f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.8f, 0.773f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)},
				{glm::vec3(-0.25f, -0.25f, 0.0f),glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.8f, 0.885f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)} };


int main()
{
	vertexMap['1'] = oneVa;
	vertexMap['2'] = twoVa;
	vertexMap['3'] = threeVa;
	vertexMap['4'] = fourVa;
	vertexMap['5'] = fiveVa;
	vertexMap['6'] = sixVa;
	vertexMap['7'] = sevenVa;
	vertexMap['8'] = eightVa;
	vertexMap['9'] = nineVa;
	vertexMap['0'] = zeroVa;

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
	glfwSetFramebufferSizeCallback(window, frameBufferSizeCallback);
	ImGuiProcesses(window);

	unsigned int ammoNum = 500;

	Shader mainShader("shaders/pointshadow.glsl", false);
	Shader cubeMapShader("shaders/pointshadow_depth.glsl", true);

	Model backpack("assets/models/backpack/backpack.obj_model");
	Model forest("assets/models/forest/forest.obj_model");
	Model guy("assets/models/guy/bearded_guy_idle_anim.obj_model");
	Model gun("assets/models/gun/pistol.obj_model");

	Texture muzzleFlash("muzzleflash.png", "assets/textures", "texture_diffuse", false);
	Texture cross("cross.png", "assets/textures", "texture_diffuse", false);
	Texture letters("letters.png", "assets/textures", "texture_diffuse", true);

	std::unordered_map<std::string, std::shared_ptr<PhysicalEntity>> entities;
	std::unordered_map<std::string, std::shared_ptr<UIEntity>> uiEntities;
	std::unordered_map<std::string, std::shared_ptr<TextEntity>> textEntities;
	std::vector<Light> lights;

	// lights
	lights.emplace_back(glm::vec3(0.3f), glm::vec3(0.5f), glm::vec3(0.5f), glm::vec3(0.0f, 1.0f, 2.0f));
	lights.emplace_back(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.0f), cam.GetPos() + cam.GetDirection() * 0.1f);

	for (int i = 0; i < 3; i++)
	{
		lights.emplace_back(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(100.0f, 0.0f, 0.0f));
	}

	// UI entities
	uiEntities["crosshair"] = UIEntity::New(planeVertices, planeIndices, cross, "shaders/cross.glsl");
	auto singleCharEntity = UIEntity::New(oneVa, planeIndices, letters, "shaders/cross.glsl");

	// physical entities
	entities["backpack1"] = PhysicalEntity::NewFromModel(backpack);
	entities["muzzleFlash"] = PhysicalEntity::NewBasic(planeVertices, planeIndices, muzzleFlash);
	entities["forest"] = PhysicalEntity::NewFromModel(forest);
	entities["guy"] = PhysicalEntity::NewFromModel(guy);
	entities["gun"] = PhysicalEntity::NewFromModel(gun);

	entities["backpack1"]->SetPos(glm::vec3(1.10f, 0.14f, 1.15f));
	entities["backpack1"]->SetScale(glm::vec3(0.01f));
	entities["muzzleFlash"]->SetScale(glm::vec3(0.0f));
	entities["muzzleFlash"]->SetPitch(90.0f);
	entities["forest"]->SetScale(glm::vec3(0.2f));
	entities["guy"]->SetPos(glm::vec3(1.10f, 0.09f, 1.2f));
	entities["guy"]->SetScale(glm::vec3(0.05f));
	entities["guy"]->RotateVertically(90.0f);
	entities["gun"]->SetScale(glm::vec3(0.0005f));

	// texts
	textEntities["ammo"] = TextEntity::New(std::to_string(ammoNum), glm::vec3(6.5f, -4.0f, 0.0f));

	Renderer renderer(cam, mainShader);
	rendererPtr = &renderer;

	auto render = [&]() {
		renderer.Render(lights, entities, cubeMapShader);
		RenderText(textEntities, singleCharEntity);
		renderer.RenderUI(uiEntities);
		ImguiRender(lights);
		};

	auto update = [&]()
		{
			switch (gunAnimation)
			{
			case true:
				gunPitch += 1.0f;

				entities["muzzleFlash"]->SetScale(glm::vec3(0.0f));
				lights[1].SetAmbient(glm::vec3(0.0f));
				lights[1].SetDiffuse(glm::vec3(0.0f));
				lights[1].SetSpecular(glm::vec3(0.0f));

				if (gunPitch >= 20.0f)
				{
					gunAnimation = false;
					gunPitch = 0.0f;

				}
				break;
			}

			entities["gun"]->SetPos(cam.GetPos() + (cam.GetDirection() + glm::vec3(0.0f, -0.4f, 0.0f) + glm::cross(cam.GetDirection(), glm::vec3(0.0f, 1.0f, 0.0f)) * 0.5f) * 0.02f);
			entities["gun"]->SetYaw(-cam.GetYaw() + 90.0f);
			entities["gun"]->SetPitch(-cam.GetPitch() - gunPitch);
			lights[1].SetPos(cam.GetPos() + cam.GetDirection() * 0.05f);
			entities["muzzleFlash"]->SetPos(entities["gun"]->GetPos() + (cam.GetDirection() * 0.01f) + glm::vec3(0.0f, 0.003f, 0.0f));
			entities["muzzleFlash"]->SetYaw(-cam.GetYaw() + 90.0f);
			entities["muzzleFlash"]->SetPitch(-cam.GetPitch() + 90.0f);
			textEntities["ammo"]->SetStr(ammoNum);
		};

	auto controls = [&]()
		{
			processInput(window, lights, entities["muzzleFlash"], ammoNum);

			if (fpsControl)
			{
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			}
			else
			{
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			}

			glfwPollEvents();

			if (!fpsControl)
			{
				cam.SetPitch(camPitch);
				cam.SetYaw(camYaw);
			}
		};

	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime1 = currentFrame - lastFrame1;
		lastFrame1 = currentFrame;

		// controls
		controls();

		// animation and entity manipulation
		update();

		// render
		render();

		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}

void RenderText(std::unordered_map<std::string, std::shared_ptr<TextEntity>> texts, std::shared_ptr<UIEntity> singleCharEntity)
{
	for (auto& text : texts)
	{
		for (int i = 0; i < text.second->GetStr().size(); i++)
		{
			singleCharEntity->SetPos(text.second->GetPos() + i * glm::vec3(0.4, 0.0f, 0.0f));
			singleCharEntity->GetMesh().UpdateVertexData(vertexMap[text.second->GetStr()[i]], 4);
			singleCharEntity->Draw();
		}
	}
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

void processInput(GLFWwindow* window, std::vector<Light>& lights, std::shared_ptr<PhysicalEntity> flash, unsigned int& ammoNum)
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
		if (!gunAnimation && ammoNum > 0)
		{
			ammoNum -= 1;
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