#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexture;
layout (location = 2) in vec3 aNormal;

out vec2 Texture;
out vec3 Normal;
out vec3 FragPos;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

void main()
{
    FragPos = vec3(uModel * vec4(aPos, 1.0f));
    Normal = aNormal;
    Texture = aTexture;
	gl_Position = uProjection * uView * vec4(FragPos, 1.0);
}
-----
#version 330 core
out vec4 FragColor;

struct Material
{
    sampler2D diffuse;
    sampler2D specular;  
    float     shininess;    
};

struct Light {
    vec3 position;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};



in vec3 FragPos;
in vec3 Normal;
in vec2 Texture;

uniform Material material;
uniform Light light; 
uniform vec3 uCamPos;

uniform vec3 uLightPos[10];
uniform vec3 uLightColors[10];

uniform sampler2D uTexture;

void main()
{
    vec3 camDir = normalize(uCamPos - FragPos);
    vec3 norm = normalize(Normal);
    vec3 totalLight = vec3(0.0f);

    for(int i = 0; i < uLightColors.length(); i++)
    {
        if(uLightColors[i] == vec3(0.0f))
        {
            continue;
        }

        vec3 ambient = light.ambient  * texture(material.diffuse, Texture).rgb;

        vec3 lightDir = normalize(uLightPos[i] - FragPos); 
        float diff = max(dot(norm, lightDir), 0.0f);
        vec3 diffuse = light.diffuse  * diff * texture(material.diffuse, Texture).rgb;

        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(camDir, reflectDir), 0.0), material.shininess);
        vec3 specular = light.specular * spec * texture(material.specular, Texture).rgb;
        totalLight = totalLight + diffuse + ambient + specular;
    }

    vec4 result = vec4((totalLight), 1.0f);
    FragColor = result;
}