#pragma once
#include "light.hpp"
#include "cubemap.hpp"

class PointLight: public Light
{
private:
	CubeMap cubeMap;
public:
	PointLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 pos);
	CubeMap& GetDepthMap();
};