#include "directionallight.hpp"

DirectionalLight::DirectionalLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 pos) : Light(ambient, diffuse, specular, pos), shadowMap(1024 * 2, 1024 * 2, 0.1f, 25.0f)
{

}

ShadowMap& DirectionalLight::GetDepthMap()
{ 
	return shadowMap;
}
