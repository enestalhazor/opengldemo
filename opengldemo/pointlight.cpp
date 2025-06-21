#include "pointlight.hpp"

PointLight::PointLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 pos) : Light(ambient, diffuse, specular, pos), cubeMap(1024 * 2, 1024 * 2, 0.1f, 25.0f)
{

}

CubeMap& PointLight::GetDepthMap()
{
	return cubeMap;
}