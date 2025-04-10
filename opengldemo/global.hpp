#pragma once
#include "shader.hpp"
#include "vertex.hpp"
#include "texture.hpp"


struct Globals
{
	Shader* shader;
	VertexArray* vBox;
	VertexArray* vGround;
	Texture* tContainer;
	Texture* tHealth;
	Texture* t1Container;
	Texture* tBullet;
	Texture* tTarget;
};
