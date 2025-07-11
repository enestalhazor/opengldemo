#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aTextureCoord;

out vec2 TexCoord;

uniform mat4 uModel;
uniform mat4 uProjection;

void main()
{
    TexCoord = aTextureCoord;
    gl_Position = uProjection * uModel * vec4(aPos, 1.0);
}
-----
#version 330 core
out vec4 FragColor;

struct Material
{
	sampler2D texture_diffuse1;
};

uniform Material uMaterial;

in vec2 TexCoord;

void main()
{
    vec4 result = texture(uMaterial.texture_diffuse1, TexCoord);
    FragColor = result;
}