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

struct Bullet
{
	glm::vec3 Pos;
	glm::vec3 Speed;

	Bullet(glm::vec3 pos, glm::vec3 speed) : Pos(pos), Speed(speed)
	{

	}
};

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

void processInput(GLFWwindow* window, std::vector <Physical> Walls, Physical& npc, Shader& shader, VertexArray& vertex, Texture& texture, bool skill);


void moveCamera(glm::vec3 vec);

void move(Physical& obj, glm::vec3 vec);

bool firstMouse = true;

const unsigned int SCR_WIDTH = 1400;
const unsigned int SCR_HEIGHT = 1000;
int frameCount = 0;
int colorCont = 0;

int health = 100;
int npcHealth = 100;

int skillTime = 0;
bool skill = false;
std::vector<Physical> pBullets;
std::vector<Physical> tBullets;
std::vector<Physical> objs;

Camera cam;

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	CallBackParameters* c = (CallBackParameters*)glfwGetWindowUserPointer(window);
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		glm::vec3 speed = glm::normalize(cam.GetDirection());
		Physical eBullet(cam.GetPos() + speed * 1.0f, glm::vec3(0.0f, 0.0f, 1.0f), speed, c->shader, c->vertex, c->texture);
		eBullet.m_Scale = glm::vec3(0.1f, 0.1f, 0.1f);
		eBullet.m_RigidBody = glm::vec3(0.1f, 0.1f, 0.1f);
		pBullets.emplace_back(eBullet);
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

	cam.ChangeYaw(xoffset);
	cam.ChangePitch(yoffset);
}

void Create3dObj(std::vector<Physical>& objs, GLFWwindow* window, float yaw, float pitch, glm::vec3 pos, glm::vec3 color, glm::vec3 speed, glm::vec3 scale, glm::vec3 rigidBody)
{
	CallBackParameters* c = (CallBackParameters*)glfwGetWindowUserPointer(window);

	Physical obj(pos, color, speed, c->shader, c->vertex, c->texture);
	obj.m_Scale = scale;
	obj.m_Yaw = yaw;
	obj.m_Pitch = pitch;
	obj.m_RigidBody = rigidBody;
	objs.emplace_back(obj);
}

void Create3dObj(std::vector<Physical>& objs, Shader& shader, VertexArray& vertex, Texture& texture, float yaw, float pitch, glm::vec3 pos, glm::vec3 color, glm::vec3 speed, glm::vec3 scale, glm::vec3 rigidBody)
{
	Physical obj(pos, color, speed, shader, vertex, texture);
	obj.m_Scale = scale;
	obj.m_Yaw = yaw;
	obj.m_Pitch = pitch;
	obj.m_RigidBody = rigidBody;
	objs.emplace_back(obj);
}

Physical Create2dObj(Shader& shader, VertexArray& vertex, Texture& texture, glm::vec3 pos, glm::vec3 color, glm::vec3 speed, float scale_x, float scale_y)
{
	Physical obj(pos, color, speed, shader, vertex, texture);
	obj.m_Scale.x = scale_x;
	obj.m_Scale.y = scale_y;

	return obj;
}

void BulletProcess(std::vector<Physical>& bullet, Renderer renderer, Physical& Bar, const glm::vec3& vec1, float barScale_x)
{
	for (auto& bullet : bullet)
	{
		move(bullet, bullet.m_Speed * 0.5f);
		renderer.Render(bullet);

		if (glm::length(glm::vec3((vec1 - bullet.m_Pos))) < 1.5f)
		{
			if (health > 0)
			{
				health -= 1.0f;
				Bar.m_Scale.x = barScale_x * health / 100;
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
		Texture tHealth("bar.jpg");
		Texture t1Container("brick.jpeg");
		Texture tBullet("bar.jpg");
		Texture tTarget("davsan.jpeg");

		Shader shader("shader.glsl");
		tBullet.Bind();

		CallBackParameters c = { shader, vBox, t1Container };
		glfwSetWindowUserPointer(window, &c);

		// Bar
		Physical hpBar = Create2dObj(shader, vGround, tHealth, glm::vec3(0.7f, -0.9f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f), 0.05f, 0.01f);
		Physical manaBar = Create2dObj(shader, vGround, tHealth, glm::vec3(0.7f, -0.75f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f), 0.05f, 0.01f);
		// Wall
		Create3dObj(objs, window, 0, 0, glm::vec3(0.0f, 5.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(10.0f, 10.0f, 1.0f), glm::vec3(10.0f, 10.0f, 1.0f));
		Create3dObj(objs, window, 0, 0, glm::vec3(10.0f, 5.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(10.0f, 10.0f, 1.0f), glm::vec3(10.0f, 10.0f, 1.0f));
		Create3dObj(objs, window, 0, 0, glm::vec3(10.0f, 5.0f, -20.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(10.0f, 10.0f, 1.0f), glm::vec3(10.0f, 10.0f, 1.0f));
		Create3dObj(objs, window, 0, 0, glm::vec3(0.0f, 5.0f, -20.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(10.0f, 10.0f, 1.0f), glm::vec3(10.0f, 10.0f, 1.0f));
		Create3dObj(objs, window, 90.0f, 0, glm::vec3(15.0f, 5.0f, -5.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(10.0f, 10.0f, 1.0f), glm::vec3(1.0f, 10.0f, 10.0f));
		Create3dObj(objs, window, 90.0f, 0, glm::vec3(15.0f, 5.0f, -15.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(10.0f, 10.0f, 1.0f), glm::vec3(1.0f, 10.0f, 10.0f));
		Create3dObj(objs, window, 90.0f, 0, glm::vec3(-5.0f, 5.0f, -15.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(10.0f, 10.0f, 1.0f), glm::vec3(1.0f, 10.0f, 10.0f));
		// Door
		Create3dObj(objs, window, 90.0f, 0, glm::vec3(-5.0f, 5.0f, -8.3f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(3.5f, 10.0f, 1.0f), glm::vec3(1.5f, 1.5f, 2.5f));
		Create3dObj(objs, window, 90.0f, 0, glm::vec3(-5.0f, 5.0f, -2.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(3.5f, 10.0f, 1.0f), glm::vec3(1.5f, 1.5f, 2.5f));
		Create3dObj(objs, window, 90.0f, 0, glm::vec3(-5.0f, 7.5f, -5.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(3.0f, 5.0f, 1.0f), glm::vec3(1.5f, 1.5f, 5.0f));
		// Roof
		// above the door
		Create3dObj(objs, window, 0, 90.0f, glm::vec3(0.0f, 10.0f, -5.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(10.0f, 10.0f, 1.0f), glm::vec3(10.0f, 1.0f, 10.0f));
		Create3dObj(objs, window, 0, -60.0f, glm::vec3(0.0f, 13.0f, -5.0f), glm::vec3(0.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(10.0f, 11.0f, 1.0f), glm::vec3(10.0f, 1.0f, 10.0f));
		// next to the roof of door
		Create3dObj(objs, window, 0, 90.0f, glm::vec3(0.0f, 10.0f, -15.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(10.0f, 10.0f, 1.0f), glm::vec3(10.0f, 1.0f, 10.0f));
		Create3dObj(objs, window, 0, 60.0f, glm::vec3(0.0f, 13.0f, -15.0f), glm::vec3(0.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(10.0f, 11.0f, 1.0f), glm::vec3(10.0f, 1.0f, 10.0f));
		// behind the roof6
		Create3dObj(objs, window, 0, 90.0f, glm::vec3(10.0f, 10.0f, -15.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(10.0f, 10.0f, 1.0f), glm::vec3(10.0f, 1.0f, 10.0f));
		Create3dObj(objs, window, 0, 60.0f, glm::vec3(10.0f, 13.0f, -15.0f), glm::vec3(0.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(10.0f, 11.0f, 1.0f), glm::vec3(10.0f, 1.0f, 10.0f));
		// next to the roof7 
		Create3dObj(objs, window, 0, 90.0f, glm::vec3(10.0f, 10.0f, -5.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(10.0f, 10.0f, 1.0f), glm::vec3(10.0f, 1.0f, 10.0f));
		Create3dObj(objs, window, 0, -60.0f, glm::vec3(10.0f, 13.0f, -5.0f), glm::vec3(0.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(10.0f, 11.0f, 1.0f), glm::vec3(10.0f, 1.0f, 10.0f));
		// Npc
		Create3dObj(objs, shader, vGround, tHealth, 0, 0, glm::vec3(0.0f, 7.5f, -10.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.6f, 0.1f, 1.0f), glm::vec3(0.0f));
		//Obj npcHp(glm::vec3(0.0f, 7.5f, -10.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), shader, vGround, tHealth);
		//npcHp.m_Scale = glm::vec3(0.6f, 0.1f, 1.0f);
		//npcHp.m_RigidBody = glm::vec3(0.0f);
		//objs.emplace_back(npcHp);

		Create3dObj(objs, shader, vBox, tTarget, 0, 0, glm::vec3(0.0f, 2.5f, -10.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(2.0f, 5.0f, 2.0f), glm::vec3(2.0f, 5.0f, 2.0f));
		Physical& npc = objs.at(objs.size() - 1);
		Physical& npcHpRef = objs.at(objs.size() - 2);

		glEnable(GL_DEPTH_TEST);

		while (!glfwWindowShouldClose(window))
		{
			double currentFrame = glfwGetTime();
			deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;

			processInput(window, objs, npc, shader, vBox, t1Container, skill);

			glClearColor(0.2f, 0.4f, 0.7f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


			if (skillTime == 100)
			{
				skill = true;
				skillTime = 0;
			}

			if (cam.GetPos().y > 2)
			{
				moveCamera(glm::vec3(0.0f, -0.1f, 0.0f));
			}

			if (npc.m_Pos.y > 2)
			{
				move(npc, glm::vec3(0.0f, -0.1f, 0.0f));
			}

			RenderGround(renderer, vGround, shader, tContainer, 50, 50);

			for (auto& wall : objs)
			{
				renderer.Render(wall);
			}

			move(npc, (cam.GetPos() - npc.m_Pos) * 0.01f);
			npcHpRef.m_Pos = npc.m_Pos + glm::vec3(0.0f, 5.0f, 0.0f);
			npcHpRef.m_Yaw = 90 - cam.GetYaw();

			if (glm::length(cam.GetPos() - npc.m_Pos) < 5)
			{
				npc.m_Pos = glm::vec3(fRand(-20, 20), 2.5f, fRand(-20, 20));
			}

			if (frameCount % 5 == 0)
			{
				//glm::vec3 speed =;
				glm::vec3 speed = glm::normalize(cam.GetPos() - npc.m_Pos) + glm::normalize(glm::vec3(fRand(-1.0f, 1.0f), fRand(-1.0f, 1.0f), fRand(-1.0f, 1.0f))) * 0.2f;
				Physical eBullet(npc.m_Pos + speed * 2.0f, glm::vec3(1.0f, 0.1f, 0.1f), speed, shader, vBox, tBullet);
				eBullet.m_Scale = glm::vec3(0.1f, 0.1f, 0.1f);
				eBullet.m_RigidBody = glm::vec3(0.1f, 0.1f, 0.1f);
				tBullets.emplace_back(eBullet);
			}

			BulletProcess(tBullets, renderer, hpBar, cam.GetPos(), 0.05f);
			BulletProcess(pBullets, renderer, npcHpRef, npc.m_Pos, 0.6f);

			skill = false;
			frameCount++;
			skillTime++;

			renderer.RenderUI(hpBar);
			renderer.RenderUI(manaBar);

			glfwSwapBuffers(window);
			glfwPollEvents();
		}
	}
	glfwTerminate();
	return 0;
}

bool detectCollisionForCamera(const Physical& obj)
{
	glm::vec3 eMin = glm::vec3(obj.m_RigidBody.x / -2, obj.m_RigidBody.y / -2, obj.m_RigidBody.z / -2) + glm::vec3(obj.m_Pos);
	glm::vec3 eMax = glm::vec3(obj.m_RigidBody.x / 2, obj.m_RigidBody.y / 2, obj.m_RigidBody.z / 2) + glm::vec3(obj.m_Pos);

	glm::vec3 cMin = glm::vec3(cam.m_RigidBody.x / -2, cam.m_RigidBody.y / -2, cam.m_RigidBody.z / -2) + cam.GetPos();
	glm::vec3 cMax = glm::vec3(cam.m_RigidBody.x / 2, cam.m_RigidBody.y / 2, cam.m_RigidBody.z / 2) + cam.GetPos();

	if (cMin.x < eMax.x &&
		cMax.x > eMin.x &&
		cMin.y < eMax.y &&
		cMax.y > eMin.y &&
		cMin.z < eMax.z &&
		cMax.z > eMin.z)
	{
		return true;
	}

	return false;
}

void move(Physical& target, glm::vec3 vec)
{
	target.m_Pos += vec;


	for (const auto& obj : objs)
	{
		if (obj.m_Id == target.m_Id)
		{
			continue;
		}

		glm::vec3 eMin = glm::vec3(obj.m_RigidBody.x / -2, obj.m_RigidBody.y / -2, obj.m_RigidBody.z / -2) + glm::vec3(obj.m_Pos);
		glm::vec3 eMax = glm::vec3(obj.m_RigidBody.x / 2, obj.m_RigidBody.y / 2, obj.m_RigidBody.z / 2) + glm::vec3(obj.m_Pos);

		glm::vec3 cMin = glm::vec3(target.m_RigidBody.x / -2, target.m_RigidBody.y / -2, target.m_RigidBody.z / -2) + glm::vec3(target.m_Pos);
		glm::vec3 cMax = glm::vec3(target.m_RigidBody.x / 2, target.m_RigidBody.y / 2, target.m_RigidBody.z / 2) + glm::vec3(target.m_Pos);

		if (cMin.x < eMax.x &&
			cMax.x > eMin.x &&
			cMin.y < eMax.y &&
			cMax.y > eMin.y &&
			cMin.z < eMax.z &&
			cMax.z > eMin.z)
		{
			target.m_Pos -= vec;
			break;
		}
	}
}

void moveCamera(glm::vec3 vec)
{
	cam.SetPos(cam.GetPos() + vec);

	for (const auto& obj : objs)
	{
		glm::vec3 eMin = glm::vec3(obj.m_RigidBody.x / -2, obj.m_RigidBody.y / -2, obj.m_RigidBody.z / -2) + glm::vec3(obj.m_Pos);
		glm::vec3 eMax = glm::vec3(obj.m_RigidBody.x / 2, obj.m_RigidBody.y / 2, obj.m_RigidBody.z / 2) + glm::vec3(obj.m_Pos);

		glm::vec3 cMin = glm::vec3(cam.m_RigidBody.x / -2, cam.m_RigidBody.y / -2, cam.m_RigidBody.z / -2) + glm::vec3(cam.GetPos());
		glm::vec3 cMax = glm::vec3(cam.m_RigidBody.x / 2, cam.m_RigidBody.y / 2, cam.m_RigidBody.z / 2) + glm::vec3(cam.GetPos());

		if (cMin.x <= eMax.x &&
			cMax.x >= eMin.x &&
			cMin.y <= eMax.y &&
			cMax.y >= eMin.y &&
			cMin.z <= eMax.z &&
			cMax.z >= eMin.z)
		{
			cam.SetPos(cam.GetPos() - vec);
			break;
		}
	}
}


void processInput(GLFWwindow* window, std::vector <Physical> Walls, Physical& npc, Shader& shader, VertexArray& vertex, Texture& texture, bool skill)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	const float cameraSpeed = deltaTime * 30.0f; // adjust accordingly
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		moveCamera(cam.GetDirection() * cameraSpeed);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		moveCamera(glm::normalize(glm::cross(cam.GetDirection(), glm::vec3(0.0f, 1.0f, 0.0f))) * -cameraSpeed);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		moveCamera(cam.GetDirection() * -cameraSpeed);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		moveCamera(glm::normalize(glm::cross(cam.GetDirection(), glm::vec3(0.0f, 1.0f, 0.0f))) * cameraSpeed);
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		cam.SetPos(cam.GetPos() + glm::vec3(0.0f, 0.3f, 0.0f));
		for (const auto& wall : Walls)
		{
			if (detectCollisionForCamera(wall))
			{
				cam.SetPos(cam.GetPos() - glm::vec3(0.0f, 0.3f, 0.0f));
				break;
			}
		}
	}
	if (cam.GetPos().y < 2)
	{
		cam.SetPos(glm::vec3(cam.GetPos().x, 2.0f, cam.GetPos().z));
	}

	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
	{
		move(npc, glm::vec3(0.0f, 0.3f, 0.0f));
	}

	if (skill == true)
	{
		if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS)
		{
			CallBackParameters* c = (CallBackParameters*)glfwGetWindowUserPointer(window);

			glm::vec3 speed = glm::normalize(cam.GetDirection());
			Physical eBullet(cam.GetPos() + speed * 1.0f, glm::vec3(0.0f, 0.0f, 1.0f), speed, c->shader, c->vertex, c->texture);
			eBullet.m_Scale = glm::vec3(1.0f, 1.0f, 1.0f);
			eBullet.m_RigidBody = glm::vec3(0.1f, 0.1f, 0.1f);
			pBullets.emplace_back(eBullet);
		}
	}
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}