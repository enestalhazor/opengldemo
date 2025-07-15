#pragma once
#include "entity.hpp"

class TextEntity : public Entity
{
private:
	std::string m_Str;
public: 
	TextEntity(std::string str, glm::vec3 pos);
	const std::string& GetStr();
	void SetStr(const std::string& str);
	void SetStr(int value);
	static std::shared_ptr<TextEntity> New(std::string str, glm::vec3 pos);
};