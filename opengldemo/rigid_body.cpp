#include "rigid_body.hpp"

RigidBody::RigidBody() : Entity(), m_CollisionBox(0)
{

}

RigidBody::RigidBody(const glm::vec3& pos, const glm::vec3& collisionBox) : Entity(pos), m_CollisionBox(collisionBox)
{

}

bool RigidBody::IsColliding(const std::vector<	RigidBody>& bodies)
{
	for (const auto& body : bodies)
	{
		if (body.m_Id == m_Id)
		{
			continue;
		}

		glm::vec3 eMin = glm::vec3(body.m_CollisionBox.x / -2, body.m_CollisionBox.y / -2, body.m_CollisionBox.z / -2) + glm::vec3(body.m_Pos);
		glm::vec3 eMax = glm::vec3(body.m_CollisionBox.x / 2, body.m_CollisionBox.y / 2, body.m_CollisionBox.z / 2) + glm::vec3(body.m_Pos);

		glm::vec3 cMin = glm::vec3(m_CollisionBox.x / -2, m_CollisionBox.y / -2, m_CollisionBox.z / -2) + m_Pos;
		glm::vec3 cMax = glm::vec3(m_CollisionBox.x / 2, m_CollisionBox.y / 2, m_CollisionBox.z / 2) + m_Pos;

		if (cMin.x < eMax.x &&
			cMax.x > eMin.x &&
			cMin.y < eMax.y &&
			cMax.y > eMin.y &&
			cMin.z < eMax.z &&
			cMax.z > eMin.z)
		{
			return true;
		}
	}

	return false;
}

void RigidBody::MoveWithCollision(glm::vec3 delta, const std::vector<RigidBody>& bodies)
{
	Move(delta);
	if (IsColliding(bodies))
	{
		Move(-delta);
	}
}