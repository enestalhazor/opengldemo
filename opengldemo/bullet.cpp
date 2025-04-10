#include "bullet.hpp"
#include "global.hpp"
#include "material.hpp"
#include "texture.hpp"

extern Globals globals;

Bullet::Bullet(glm::vec3 pos, glm::vec3 speed, const RigidBody& target) : Physical(pos, glm::vec3(0.1f, 0.1f, 0.1f), *globals.shader, *globals.vBox, *globals.tBullet, { Texture("container.jpg"), Texture("container.jpg"),32.0f })
{
	SetScale(glm::vec3(0.1f));
	SetSpeed(speed);
}