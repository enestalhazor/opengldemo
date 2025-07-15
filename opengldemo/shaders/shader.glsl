#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTextureCoord;

out vec2 TexCoord;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

void main()
{
    TexCoord = aTextureCoord;
    gl_Position = uProjection * uView * uModel * vec4(aPos, 1.0);
}
-----
#version 330 core
out vec4 FragColor;
  
in vec2 TexCoord;
uniform vec4 uColor;
uniform sampler2D uTexture;

void main()
{
    FragColor = texture(uTexture, TexCoord) * uColor;
}