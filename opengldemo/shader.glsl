#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTextureCoord;

out vec3 ourColor;
out vec2 TexCoord;

uniform mat4 uTransform;

void main()
{
    ourColor = aColor;
    TexCoord = aTextureCoord;
    gl_Position = uTransform * vec4(aPos, 1.0);
}
-----
#version 330 core
out vec4 FragColor;
  
in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D ourTexture;

void main()
{
    FragColor = texture(ourTexture, TexCoord) * vec4(ourColor, 1.0);
}