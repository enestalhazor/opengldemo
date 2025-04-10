#pragma once
#include <iostream>
#include <glm/glm.hpp>
#include "texture.hpp"

struct Material
{
	const Texture& diffuseMap;
	const Texture& specularMap;
	float shininess;
};