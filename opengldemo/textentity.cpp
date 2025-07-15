#include "textentity.hpp"

TextEntity::TextEntity(std::string str, glm::vec3 pos) : m_Str(str), Entity(pos)
{

}

const std::string& TextEntity::GetStr()
{
	return m_Str;
}

void TextEntity::SetStr(const std::string& str)
{
	m_Str = str;
}

void TextEntity::SetStr(int value)
{
	m_Str = std::to_string(value);
}

std::shared_ptr<TextEntity> TextEntity::New(std::string str, glm::vec3 pos)
{
	return std::make_shared<TextEntity>(str, pos);
}

