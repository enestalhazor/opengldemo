#include "light.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Light::Light(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 pos) : Entity(pos), m_Ambient(ambient), m_Diffuse(diffuse), m_Specular(specular)
{

}

glm::vec3 Light::GetAmbient()
{
	return m_Ambient;
}

glm::vec3 Light::GetDiffuse()
{
	return m_Diffuse;
}

glm::vec3 Light::GetSpecular()
{
	return m_Specular;
}
