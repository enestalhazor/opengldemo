#pragma once
#include "light.hpp"
#include "shadowmap.hpp"

class DirectionalLight : public Light
{
private:
	ShadowMap shadowMap;
public:
	DirectionalLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 pos);
	ShadowMap& GetDepthMap();
};