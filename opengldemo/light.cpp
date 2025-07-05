#include "light.hpp"

const unsigned int SHADOW_WIDTH = 1024 * 4;
const unsigned int SHADOW_HEIGHT = 1024 * 4;

Light::Light(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 pos, std::vector<Mesh>& meshes):
PhysicalEntity(meshes, pos), m_CubeMap(SHADOW_WIDTH, SHADOW_HEIGHT, 0.5f, 25.0f), m_Ambient(ambient), m_Diffuse(diffuse), m_Specular(specular), m_Pos(pos)
{

}

glm::vec3& Light::GetAmbient()
{
	return m_Ambient;
}

glm::vec3& Light::GetDiffuse()
{
	return m_Diffuse;
}

glm::vec3& Light::GetSpecular()
{
	return m_Specular;
}

void Light::SetAmbient(const glm::vec3& ambient)
{
	m_Ambient = ambient;
}
void Light::SetDiffuse(const glm::vec3& diffuse)
{
	m_Diffuse = diffuse;
}

void Light::SetSpecular(const glm::vec3& specular)
{
	m_Specular = specular;
}

CubeMap& Light::GetCubeMap()
{
	return m_CubeMap;
}