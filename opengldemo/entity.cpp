#include "entity.hpp"

unsigned int Entity::counter = 0;

Entity::Entity() : Entity(glm::vec3(0.0f))
{
}

Entity::Entity(glm::vec3 pos) : m_Id(counter++), m_Pos(pos), m_Scale(1), m_Speed(0), m_Direction(0.0f, 0.0f, 1.0f), m_Pitch(0), m_Yaw(0)
{

}

glm::vec3 Entity::getPos() const
{
	return m_Pos;
}

void Entity::Move()
{
	m_Pos += m_Speed;
}

void Entity::Move(glm::vec3 delta)
{
	m_Pos += delta;
}

void Entity::RotateHorizontally(float degree)
{
	m_Yaw += degree;
	calculateDirection();
}

void Entity::RotateVertically(float degree)
{
	m_Pitch += degree;
	calculateDirection();
}

unsigned int Entity::GetId() const
{
	return m_Id;
}

glm::vec3 Entity::GetDirection() const
{
	return m_Direction;
}

glm::vec3 Entity::GetScale() const
{
	return m_Scale;
}

float  Entity::GetYaw() const
{

}

float Entity::GetPitch() const
{

}

void Entity::SetSpeed(glm::vec3 speed)
{
	m_Speed = speed;
}

void Entity::SetScale(glm::vec3 scale)
{
	m_Scale = scale;
}

float Entity::DistanceFrom(const Entity& entity) const
{
	return glm::length(entity.m_Pos - m_Pos);
}

void Entity::MoveForward(float amount)
{
	m_Pos += amount * m_Direction;
}

void Entity::MoveBackward(float amount)
{
	MoveForward(-amount);
}

void Entity::MoveRight(float amount)
{
	m_Pos += glm::normalize(glm::cross(m_Direction, glm::vec3(0.0f, 1.0f, 0.0f))) * amount;
}

void Entity::MoveLeft(float amount)
{
	MoveRight(-amount);
}

void Entity::calculateDirection()
{
	if (m_Pitch > 89.0f)
		m_Pitch = 89.0f;
	if (m_Pitch < -89.0f)
		m_Pitch = -89.0f;

	glm::vec3 direction;
	direction.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
	direction.y = sin(glm::radians(m_Pitch));
	direction.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
	m_Direction = glm::normalize(direction);
}
