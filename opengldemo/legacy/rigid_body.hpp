#pragma once
#include <glm/glm.hpp>
#include <vector>
#include "entity.hpp"

class RigidBody : public Entity
{
protected:
	glm::vec3 m_CollisionBox;
public:
	RigidBody();
	RigidBody(const glm::vec3& pos, const glm::vec3& collisionBox);
	bool IsColliding(const RigidBody& body);

	template<typename T>
	void MoveWithoutCollision(glm::vec3 delta, const std::vector<T>& bodies)
	{
		Move(delta);

		for (const RigidBody& wall : bodies)
		{
			if (IsColliding(wall))
			{
				Move(-delta);
				break;
			}
		}
	}

};