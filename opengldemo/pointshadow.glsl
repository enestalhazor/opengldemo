#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

uniform mat4 uProjection;
uniform mat4 uView;
uniform mat4 uModel;

void main()
{
	FragPos = vec3(uModel * vec4(aPos, 1.0f));
	Normal = inverse(mat3(uModel)) * aNormal;
	TexCoords = aTexCoords;
	gl_Position = uProjection * uView * uModel * vec4(aPos, 1.0f);
}
-----
#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform vec3 uViewPos;
uniform float uFar_plane;
uniform samplerCube uDepthMap;

struct Material
{
	sampler2D texture_diffuse1;
	sampler2D texture_specular1;
	sampler2D texture_mormal1;
};

struct Light
{
	vec3 position;
	vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Material uMaterial;
uniform Light uLights[2];

float ShadowCalculation(vec3 fragPos)
{
    vec3 fragToLight = FragPos - light.position;
    float closest = texture(uDepthMap, fragToLight).r;
    closest *= uFar_plane;
    float currentDepth = length(fragToLight);
    float bias = 0.05f;
    float shadow = currentDepth -  bias > closest ? 1.0f : 0.0f;        
        
    return shadow;
}

void main()
{           
    vec3 color = texture(material.texture_diffuse1, TexCoords).rgb;
    vec3 normal = normalize(Normal);
    vec3 lightColor = vec3(1.0f);

    vec3 ambient = light.ambient * texture(material.texture_diffuse1, TexCoords).rgb;

    vec3 lightDir = normalize(light.position- FragPos);
    float diff = max(dot(lightDir, normal), 0.0f);
    vec3 diffuse = light.diffuse * diff * texture(material.texture_diffuse1, TexCoords).rgb;

    vec3 viewDir = normalize(uViewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = 0.0f;
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    spec = pow(max(dot(normal, halfwayDir), 0.0f), 64.0f);
    vec3 specular = spec * texture(material.texture_specular1, TexCoords).rgb;    

    float shadow = ShadowCalculation(FragPos);                      
    vec3 lighting = (ambient + (1.0f - shadow) * (diffuse + specular)) * color;    
    
    FragColor = vec4(lighting, 1.0f);
}