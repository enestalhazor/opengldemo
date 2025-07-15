#pragma once
#include <glm/glm.hpp>
#include "vertex_array.hpp"
#include "shader.hpp"
#include "texture.hpp"

class Entity
{
protected:
	unsigned int m_Id;
	glm::vec3 m_Pos;
	glm::vec3 m_Speed;
	glm::vec3 m_Scale;
	glm::vec3 m_Direction;
	float m_Yaw;
	float m_Pitch;

public:

	static unsigned int counter;

	Entity();
	Entity(glm::vec3 pos);
	glm::vec3& GetPos(); 
	// moves the entity using it speed
	void Move();
	// moves the entity by delta
	void Move(glm::vec3 delta);
	void RotateHorizontally(float degree);
	void RotateVertically(float degree);
	unsigned int GetId() const;
	glm::vec3 GetDirection() const;
	glm::vec3 GetScale() const;
	glm::vec3 GetSpeed() const;
	float GetYaw() const;
	float GetPitch() const;
	void SetSpeed(glm::vec3 speed);
	void SetYaw(float yaw);
	void SetPitch(float pitch);
	void SetPos(glm::vec3 pos);
	void SetScale(glm::vec3 scale);
	float DistanceFrom(const Entity& entity) const;
	void MoveForward(float amount);
	void MoveBackward(float amount);
	void MoveRight(float amount);
	void MoveLeft(float amount);
	glm::mat4 GetModelMatrix();

private:
	void calculateDirection();
};

