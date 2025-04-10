#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

void main()
{
	gl_Position = uProjection * uView * uModel * vec4(aPos, 1.0);
}
-----
#version 330 core
out vec4 FragColor;

void main()
{
    FragColor = glm::vec4(1.0f); // set all 4 vector values to 1.0
}