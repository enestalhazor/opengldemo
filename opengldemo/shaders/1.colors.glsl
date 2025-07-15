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

uniform Material uMaterial;
uniform Light uLights[5]; 
uniform vec3 uCamPos;

uniform sampler2D uTexture;

void main()
{
    vec3 camDir = normalize(uCamPos - FragPos);
    vec3 norm = normalize(Normal);
    vec3 totalLight = vec3(0.0f);

    for(int i = 0; i < uLights.length(); i++)
    {
        Light light = uLights[i];

        vec3 ambient = light.ambient  * texture(uMaterial.diffuse, Texture).rgb;

        vec3 lightDir = normalize(light.position - FragPos); 
        float diff = max(dot(norm, lightDir), 0.0f);
        vec3 diffuse = light.diffuse  * diff * texture(uMaterial.diffuse, Texture).rgb;

        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(camDir, reflectDir), 0.0), uMaterial.shininess);
        vec3 specular = light.specular * spec * texture(uMaterial.specular, Texture).rgb;
        totalLight = totalLight + diffuse + ambient + specular;
    }

    vec4 result = vec4((totalLight), 1.0f);
    FragColor = result;
}