/*#define _CRT_SECURE_NO_WARNINGS

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
#include "physical.hpp"
#include "vertex_data.hpp"
#include "global.hpp"
#include "bullet.hpp"

struct Wall
{
	glm::mat4 Model;

	Wall(glm::mat4 model) : Model(model)
	{

	}
};

struct CallBackParameters
{
	Shader& shader;
	VertexArray& vertex;
	Texture& texture;
	RigidBody& target;
};

float fRand(float fMin, float fMax)
{
	float f = (float)rand() / (float)RAND_MAX;
	return fMin + f * (fMax - fMin);
}


float deltaTime = 0.0f;
float lastFrame = 0.0f;
float lastX;
float lastY;
int score = 0;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void processInput(GLFWwindow* window, Physical& npc, Shader& shader, VertexArray& vertex, Texture& texture, bool skill);

bool firstMouse = true;

const unsigned int SCR_WIDTH = 1400;
const unsigned int SCR_HEIGHT = 1000;
int frameCount = 0;
int colorCont = 0;

int health = 100;
int npcHealth = 100;

int skillTime = 0;
bool skill = false;
std::vector<Bullet> pBullets;
std::vector<Bullet> tBullets;
std::vector<Physical> walls;

Camera cam(glm::vec3(20.0f, 20.0f, 5.0f));

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	CallBackParameters* c = (CallBackParameters*)glfwGetWindowUserPointer(window);
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		glm::vec3 speed = glm::normalize(cam.GetDirection());
		pBullets.emplace_back(cam.GetPos() + speed * 1.0f, speed, c->target);
	}

}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.2f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	cam.RotateHorizontally(xoffset);
	cam.RotateVertically(yoffset);
}


void Create3dObj(std::vector<Physical>& walls, GLFWwindow* window, float yaw, float pitch, glm::vec3 pos, glm::vec3 color, glm::vec3 speed, glm::vec3 scale, glm::vec3 rigidBody)
{
	CallBackParameters* c = (CallBackParameters*)glfwGetWindowUserPointer(window);

	Physical obj(pos, rigidBody, c->shader, c->vertex, c->texture, { Texture("container.jpg"), Texture("container.jpg"), 32.0f });
	obj.SetScale(scale);
	obj.SetYaw(yaw);
	obj.SetPitch(pitch);
	obj.SetSpeed(speed);
	walls.emplace_back(obj);
}

void Create3dObj(std::vector<Physical>& walls, Shader& shader, VertexArray& vertex, Texture& texture, float yaw, float pitch, glm::vec3 pos, glm::vec3 color, glm::vec3 speed, glm::vec3 scale, glm::vec3 rigidBody)
{
	Physical obj(pos, rigidBody, shader, vertex, texture, { Texture("container.jpg"), Texture("container.jpg"), 32.0f });
	obj.SetScale(scale);
	obj.SetYaw(yaw);
	obj.SetPitch(pitch);
	obj.SetSpeed(speed);
	walls.emplace_back(obj);
}

Physical Create2dObj(Shader& shader, VertexArray& vertex, Texture& texture, glm::vec3 pos, glm::vec3 color, glm::vec3 speed, float scale_x, float scale_y)
{
	Physical obj(pos, speed, shader, vertex, texture, { Texture("container.jpg"), Texture("container.jpg"), 32.0f });
	obj.SetScale(glm::vec3(scale_x, scale_y, 1.0f));
	return obj;
}

void BulletProcess(std::vector<Bullet>& bullets, Renderer renderer, Physical& bar, const RigidBody& target, float barScale_x, int& health)
{
	for (auto& bullet : bullets)
	{
		bullet.MoveWithoutCollision(bullet.GetSpeed(), walls);

		renderer.Render(bullet);

		if (bullet.DistanceFrom(target) < 1.5f)
		{
			if (health > 0)
			{
				health -= 1.0f;
				bar.SetScale(glm::vec3(barScale_x * health / 100, bar.GetScale().y, bar.GetScale().z));
			}
		}
	}
}

void RenderGround(Renderer& renderer, VertexArray& vertex, Shader& shader, Texture& texture, int width, int height)
{
	for (int i = 0; i < height; i++)
	{

		for (int j = 0; j < width; j++)
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(i * -10 + 100, 0.0f, j * -10 + 95));
			model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			renderer.Render(vertex, shader, texture, model, glm::vec4(0.6f, 0.6f, 0.9f, 1.0f));
		}
	}
}

void CreateHouse(GLFWwindow* window)
{
	// Wall
	Create3dObj(walls, window, 0, 0, glm::vec3(0.0f, 5.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(10.0f, 10.0f, 1.0f), glm::vec3(10.0f, 10.0f, 1.0f));
	Create3dObj(walls, window, 0, 0, glm::vec3(10.0f, 5.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(10.0f, 10.0f, 1.0f), glm::vec3(10.0f, 10.0f, 1.0f));
	Create3dObj(walls, window, 0, 0, glm::vec3(10.0f, 5.0f, -20.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(10.0f, 10.0f, 1.0f), glm::vec3(10.0f, 10.0f, 1.0f));
	Create3dObj(walls, window, 0, 0, glm::vec3(0.0f, 5.0f, -20.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(10.0f, 10.0f, 1.0f), glm::vec3(10.0f, 10.0f, 1.0f));
	Create3dObj(walls, window, 0, 0, glm::vec3(15.0f, 5.0f, -5.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 10.0f, 10.0f), glm::vec3(1.0f, 10.0f, 10.0f));
	Create3dObj(walls, window, 0, 0, glm::vec3(15.0f, 5.0f, -15.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 10.0f, 10.0f), glm::vec3(1.0f, 10.0f, 10.0f));
	Create3dObj(walls, window, 0, 0, glm::vec3(-5.0f, 5.0f, -15.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 10.0f, 10.0f), glm::vec3(1.0f, 10.0f, 10.0f));
	// Door
	Create3dObj(walls, window, 90.0f, 0, glm::vec3(-5.0f, 5.0f, -8.3f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(3.5f, 10.0f, 1.0f), glm::vec3(1.5f, 1.5f, 2.5f));
	Create3dObj(walls, window, 90.0f, 0, glm::vec3(-5.0f, 5.0f, -2.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(3.5f, 10.0f, 1.0f), glm::vec3(1.5f, 1.5f, 2.5f));
	Create3dObj(walls, window, 90.0f, 0, glm::vec3(-5.0f, 7.5f, -5.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(3.0f, 5.0f, 1.0f), glm::vec3(1.5f, 1.5f, 5.0f));
	// Roof
	// above the door
	Create3dObj(walls, window, 0, 90.0f, glm::vec3(0.0f, 10.0f, -5.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(10.0f, 10.0f, 1.0f), glm::vec3(10.0f, 1.0f, 10.0f));
	Create3dObj(walls, window, 0, -60.0f, glm::vec3(0.0f, 13.0f, -5.0f), glm::vec3(0.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(10.0f, 11.0f, 1.0f), glm::vec3(10.0f, 1.0f, 10.0f));
	// next to the roof of door
	Create3dObj(walls, window, 0, 90.0f, glm::vec3(0.0f, 10.0f, -15.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(10.0f, 10.0f, 1.0f), glm::vec3(10.0f, 1.0f, 10.0f));
	Create3dObj(walls, window, 0, 60.0f, glm::vec3(0.0f, 13.0f, -15.0f), glm::vec3(0.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(10.0f, 11.0f, 1.0f), glm::vec3(10.0f, 1.0f, 10.0f));
	// behind the roof6
	Create3dObj(walls, window, 0, 90.0f, glm::vec3(10.0f, 10.0f, -15.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(10.0f, 10.0f, 1.0f), glm::vec3(10.0f, 1.0f, 10.0f));
	Create3dObj(walls, window, 0, 60.0f, glm::vec3(10.0f, 13.0f, -15.0f), glm::vec3(0.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(10.0f, 11.0f, 1.0f), glm::vec3(10.0f, 1.0f, 10.0f));
	// next to the roof7 
	Create3dObj(walls, window, 0, 90.0f, glm::vec3(10.0f, 10.0f, -5.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(10.0f, 10.0f, 1.0f), glm::vec3(10.0f, 1.0f, 10.0f));
	Create3dObj(walls, window, 0, -60.0f, glm::vec3(10.0f, 13.0f, -5.0f), glm::vec3(0.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(10.0f, 11.0f, 1.0f), glm::vec3(10.0f, 1.0f, 10.0f));
}

Globals globals = { nullptr, nullptr , nullptr , nullptr , nullptr , nullptr , nullptr , nullptr };

int main2()
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

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	if (glfwRawMouseMotionSupported())
	{
		glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
	}

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, cursor_position_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);

	{
		VertexArray vGround(groundVertices, indices, 6, sizeof(float) * 5, 6);
		VertexArray vBox(boxVertices, indices, 36, sizeof(float) * 5, 6);
		Renderer renderer(cam);

		Texture tContainer("container.jpg");
		Texture tHealth("Bar.jpg");
		Texture t1Container("brick.jpeg");
		Texture tBullet("Bar.jpg");
		Texture tTarget("davsan.jpeg");

		Shader shader("shader.glsl");

		globals.shader = &shader;
		globals.vGround = &vGround;
		globals.vBox = &vBox;
		globals.tContainer = &tContainer;
		globals.t1Container = &t1Container;
		globals.tHealth = &tHealth;
		globals.tBullet = &tBullet;
		globals.tTarget = &tTarget;

		tBullet.Bind();

		// Bar
		Physical hpBar = Create2dObj(shader, vGround, tHealth, glm::vec3(0.7f, -0.9f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f), 0.05f, 0.01f);
		Physical manaBar = Create2dObj(shader, vGround, tHealth, glm::vec3(0.7f, -0.75f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f), 0.05f, 0.01f);


		// Npc
		Create3dObj(walls, shader, vGround, tHealth, 0, 0, cam.GetPos() + glm::vec3(70.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.6f, 0.1f, 1.0f), glm::vec3(0.0f));
		//Obj npcHp(glm::vec3(0.0f, 7.5f, -10.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), shader, vGround, tHealth);
		//npcHp.m_Scale = glm::vec3(0.6f, 0.1f, 1.0f);
		//npcHp.m_RigidBody = glm::vec3(0.0f);
		//objs.emplace_back(npcHp);

		Create3dObj(walls, shader, vBox, tTarget, 0, 0, cam.GetPos() + glm::vec3(70.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(2.0f, 5.0f, 2.0f), glm::vec3(2.0f, 5.0f, 2.0f));
		Physical& npc = walls.at(walls.size() - 1);
		Physical& npcHpRef = walls.at(walls.size() - 2);

		glEnable(GL_DEPTH_TEST);

		CallBackParameters c = { shader, vBox, t1Container, npc };
		glfwSetWindowUserPointer(window, &c);

		CreateHouse(window);

		while (!glfwWindowShouldClose(window))
		{
			double currentFrame = glfwGetTime();
			deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;

			processInput(window, npc, shader, vBox, t1Container, skill);

			glClearColor(0.2f, 0.4f, 0.7f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


			if (skillTime == 100)
			{
				skill = true;
				skillTime = 0;
			}

			if (cam.GetPos().y > 2)
			{
				cam.MoveWithoutCollision(glm::vec3(0.0f, -0.1f, 0.0f), walls);
			}

			if (npc.GetPos().y > 2)
			{
				cam.MoveWithoutCollision(glm::vec3(0.0f, -0.1f, 0.0f), walls);
			}

			RenderGround(renderer, vGround, shader, tContainer, 50, 50);

			for (auto& wall : walls)
			{
				renderer.Render(wall);
			}

			npc.MoveWithoutCollision((cam.GetPos() - npc.GetPos()) * 0.01f, walls);
			npcHpRef.SetPos(npc.GetPos() + glm::vec3(0.0f, 5.0f, 0.0f));
			npcHpRef.SetYaw(90 - cam.GetYaw());

			if (glm::length(cam.GetPos() - npc.GetPos()) < 5)
			{
				npc.SetPos(glm::vec3(fRand(-20, 20), 2.5f, fRand(-20, 20)));
			}

			if (frameCount % 5 == 0)
			{
				//glm::vec3 speed =;
				glm::vec3 speed = glm::normalize(cam.GetPos() - npc.GetPos()) + glm::normalize(glm::vec3(fRand(-1.0f, 1.0f), fRand(-1.0f, 1.0f), fRand(-1.0f, 1.0f))) * 0.2f;
				tBullets.emplace_back(npc.GetPos() + speed * 2.0f, speed, cam);
			}

			BulletProcess(tBullets, renderer, hpBar, cam, 0.05f, health);
			BulletProcess(pBullets, renderer, npcHpRef, npc, 0.6f, npcHealth);

			skill = false;
			frameCount++;
			skillTime++;

			renderer.RenderUI(hpBar);
			renderer.RenderUI(manaBar);

			glfwSwapBuffers(window);
			glfwPollEvents();
		}
	}
	return 0;
}

void processInput(GLFWwindow* window, Physical& npc, Shader& shader, VertexArray& vertex, Texture& texture, bool skill)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	const float cameraSpeed = deltaTime * 30.0f; // adjust accordingly
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		cam.MoveWithoutCollision(cam.GetDirection() * cameraSpeed, walls);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{

		cam.MoveWithoutCollision(glm::normalize(glm::cross(cam.GetDirection(), glm::vec3(0.0f, 1.0f, 0.0f))) * -cameraSpeed, walls);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		cam.MoveWithoutCollision(cam.GetDirection() * -cameraSpeed, walls);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		cam.MoveWithoutCollision(glm::normalize(glm::cross(cam.GetDirection(), glm::vec3(0.0f, 1.0f, 0.0f))) * cameraSpeed, walls);
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		cam.MoveWithoutCollision(glm::vec3(0.0f, 0.3f, 0.0f), walls);
	}
	if (cam.GetPos().y < 2)
	{
		cam.SetPos(glm::vec3(cam.GetPos().x, 2.0f, cam.GetPos().z));
	}

	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
	{
		npc.MoveWithoutCollision(glm::vec3(0.0f, 0.3f, 0.0f), walls);
	}

	if (skill == true)
	{
		if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS)
		{
			CallBackParameters* c = (CallBackParameters*)glfwGetWindowUserPointer(window);

			glm::vec3 speed = glm::normalize(cam.GetDirection());
			Bullet eBullet(cam.GetPos() + speed * 1.0f, speed, c->target);
			eBullet.SetScale(glm::vec3(1.0f, 1.0f, 1.0f));
			pBullets.emplace_back(eBullet);
		}
	}
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}*/