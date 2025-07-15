#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;


out vec2 TexCoords;
out vec3 FragPos;
out vec3 Normal;
out vec4 FragPosLightSpace; 

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;
uniform mat4 uLightSpaceMatrix;

void main()
{
	Normal = aNormal;
	FragPos = vec3(uModel * vec4(aPos, 1.0));
	TexCoords = aTexCoords;
	FragPosLightSpace = uLightSpaceMatrix * vec4(FragPos, 1.0f);
	gl_Position = uProjection * uView * uModel * vec4(aPos, 1.0);
}
-----
#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;
in vec4 FragPosLightSpace;

uniform sampler2D uDepthMap;
uniform vec3 uViewPos;

struct uMaterial
{
	sampler2D texture_diffuse1;
	sampler2D texture_specular1;
	sampler2D texture_mormal1;
};

struct uLight
{
	vec3 position;
	vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform uMaterial material;
uniform uLight light;

float ShadowCalculation(vec4 fragPosLightSpace)
{
    vec3 v = fragPosLightSpace.xyz;
    
    v = v * 0.5f + 0.5f; 

    float closest = texture(uDepthMap, v.xy).r;
    float current = v.z;
    
    vec3 lightDir = normalize(light.position - FragPos);
	vec3 norm = normalize(Normal);

    float bias = max(0.05 * (1.0 - dot(norm, lightDir)), 0.005);
	float shadow = 0.0f;
	vec2 texelSize = 1.0f / textureSize(uDepthMap, 0);

	for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
			float pcfDepth = texture(uDepthMap, v.xy + vec2(x, y) * texelSize).r; 
            shadow += current - bias > pcfDepth  ? 0.0 : 1.0;
		}
	}
	shadow /= 9.0f;

	if(v.z > 1.0)
	{
        shadow = 0.0;
	}

    return shadow;
}

void main()
{
	float ambientStrength = 0.3f;
    vec3 ambient = light.ambient * texture(material.texture_diffuse1, TexCoords).rgb;

	vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(material.texture_diffuse1, TexCoords).rgb;

	vec3 viewDir = normalize(uViewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), 64);
	vec3 specular = light.specular * spec * texture(material.texture_specular1, TexCoords).rgb;

	float shadow = ShadowCalculation(FragPosLightSpace);
	vec3 result = shadow * (specular + diffuse) + ambient;

    FragColor = vec4(result, 1.0f);
}