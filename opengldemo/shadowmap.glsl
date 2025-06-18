#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 uModel;
uniform mat4 lightSpaceMatrix;

void main()
{
	gl_Position = lightSpaceMatrix * uModel * vec4(aPos, 1.0);
}
-----
#version 330 core

void main()
{
	 gl_FragDepth = gl_FragCoord.z;
}