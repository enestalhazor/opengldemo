#pragma once
#include <glm/glm.hpp>
#include "physical.hpp"

class Bullet : public Physical
{
private:
	const RigidBody target;
public:
	Bullet(glm::vec3 pos, glm::vec3 speed, const RigidBody& target);
};
