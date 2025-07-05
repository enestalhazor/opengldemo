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
	Normal = (mat3(uModel)) * aNormal;
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

struct Material
{
	sampler2D texture_diffuse1;
	sampler2D texture_specular1;
	sampler2D texture_normal1;
};

struct Light
{
	vec3 position;
	vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Material uMaterial;
uniform Light uLight[3];
uniform samplerCube uDepthMap[3];

float ShadowCalculation(vec3 fragPos, int i)
{
    vec3 fragToLight = FragPos - uLight[i].position;
    float closest = texture(uDepthMap[i], fragToLight).r;
    closest *= uFar_plane;
    float currentDepth = length(fragToLight);
    float bias = 0.05f;
    float shadow = currentDepth - bias > closest ? 1.0f : 0.0f;        
    return shadow;
}

void main()
{     
    vec4 lighting = vec4(0.0f);
    for(int i = 0; i < 3; i++)
    {
        vec4 color = texture(uMaterial.texture_diffuse1, TexCoords).rgba;
        if(color.a < 0.1f)
        {
            discard;
        }

        vec3 normal = normalize(Normal);   
        vec4 lightColor = vec4(1.0f);

        vec4 ambient = vec4(uLight[i].ambient, 1.0f) * texture(uMaterial.texture_diffuse1, TexCoords).rgba;


        vec3 lightDir = normalize(uLight[i].position - FragPos);
        float diff = max(dot(lightDir, normal), 0.0f);
        vec4 diffuse = vec4(uLight[i].diffuse, 1.0f) * diff * texture(uMaterial.texture_diffuse1, TexCoords).rgba;

        vec3 viewDir = normalize(uViewPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, normal);
        float spec = 0.0f;
        vec3 halfwayDir = normalize(lightDir + viewDir);
        spec = pow(max(dot(normal, halfwayDir), 0.0f), 64.0f);
        vec4 specular = spec * texture(uMaterial.texture_specular1, TexCoords).rgba;
        float shadow = ShadowCalculation(FragPos, i);
        lighting += (ambient + (1.0f - shadow) * (diffuse + specular)) * color;
    }

    FragColor = lighting;
}