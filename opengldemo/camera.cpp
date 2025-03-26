#include "camera.hpp"


Camera::Camera() : RigidBody()
{

}

Camera::Camera(glm::vec3 pos) : RigidBody(pos, glm::vec3(2.0f, 5.0f, 2.0f))
{

}

glm::mat4 Camera::GetViewMatrix() const
{
	glm::mat4 view = glm::mat4(1.0f);
	view = glm::lookAt(m_Pos, m_Pos + m_Direction, glm::vec3(0.0f, 1.0f, 0.0f));
	return view;
}