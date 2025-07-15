//#include "rigid_body.hpp"
//
//RigidBody::RigidBody() : Entity(), m_CollisionBox(0)
//{
//
//}
//
//RigidBody::RigidBody(const glm::vec3& pos, const glm::vec3& collisionBox) : Entity(pos), m_CollisionBox(collisionBox)
//{
//
//}
//
//bool RigidBody::IsColliding(const RigidBody& body)
//{
//	if (body.m_Id == m_Id)
//	{
//		return false;
//	}
//
//	glm::vec4 aMin = glm::vec4(body.m_CollisionBox.x / -2, body.m_CollisionBox.y / -2, body.m_CollisionBox.z / -2, 1.0f);
//	glm::vec4 aMax = glm::vec4(body.m_CollisionBox.x / 2, body.m_CollisionBox.y / 2, body.m_CollisionBox.z / 2, 1.0f);
//
//	glm::vec4 bMin = glm::vec4(m_CollisionBox.x / -2, m_CollisionBox.y / -2, m_CollisionBox.z / -2, 1.0f);
//	glm::vec4 bMax = glm::vec4(m_CollisionBox.x / 2, m_CollisionBox.y / 2, m_CollisionBox.z / 2, 1.0f);
//
//	glm::mat4 t = glm::mat4(1.0f);
//	t = glm::translate(t, body.GetPos());
//
//	aMin = t * aMin;
//	aMax = t * aMax;
//
//	t = glm::mat4(1.0f);
//	t = glm::translate(t, GetPos());
//
//	bMin = t * bMin;
//	bMax = t * bMax;
//
//
//	if (bMin.x < aMax.x &&
//		bMax.x > aMin.x &&
//		bMin.y < aMax.y &&
//		bMax.y > aMin.y &&
//		bMin.z < aMax.z &&
//		bMax.z > aMin.z)
//	{
//		return true;
//	}
//	return false;
//}
//