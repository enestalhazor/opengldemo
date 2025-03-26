#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTextureCoord;

out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    TexCoord = aTextureCoord;
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
-----
#version 330 core
out vec4 FragColor;
  
in vec2 TexCoord;
uniform vec4 uColor;
uniform sampler2D ourTexture;

void main()
{
    FragColor = texture(ourTexture, TexCoord) * uColor;
}