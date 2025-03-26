#pragma once

#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "rigid_body.hpp"

class Camera : public RigidBody
{
public:

	Camera();
	Camera(glm::vec3 Pos);

	glm::mat4 GetViewMatrix() const;
};