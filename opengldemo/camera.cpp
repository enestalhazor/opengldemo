#include "camera.hpp"


Camera::Camera() : Camera(glm::vec3(0.0f))
{

}

Camera::Camera(glm::vec3 pos) : Entity(pos)
{

}

glm::mat4 Camera::GetViewMatrix() const
{
	glm::mat4 view = glm::mat4(1.0f);
	view = glm::lookAt(m_Pos, m_Pos + m_Direction, glm::vec3(0.0f, 1.0f, 0.0f));
	return view;
}