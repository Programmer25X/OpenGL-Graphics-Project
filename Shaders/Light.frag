#version 330 core

layout(location = 0) out vec4 FragColor;


struct Material
{
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct DirectionalLight
{
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight 
{
   vec3 position;

   float constant;
   float linear;
   float quadratic;  

   vec3 ambient;
   vec3 diffuse;
   vec3 specular;
};

struct SpotLight
{
   vec3 position;
   vec3 direction;

   float innerCutOff;
   float outerCutOff;

   float constant;
   float linear;
   float quadratic;

   vec3 ambient;
   vec3 diffuse;
   vec3 specular;
};

#define NR_POINT_LIGHTS 4

in vec2 v_TexCoord;
in vec3 v_normal;  
in vec3 FragPos; 

uniform vec3 u_viewPosition;
uniform Material u_material;
uniform DirectionalLight u_directionalLight; 
uniform PointLight u_pointLight[NR_POINT_LIGHTS];
uniform SpotLight u_spotLight; 

uniform bool u_isDirectionalLightEnabled = true;
uniform bool u_isPointlLightEnabled = true;
uniform bool u_isSpotlLightEnabled = true;


vec3 calculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDirection);
vec3 calculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDirection);
vec3 calculateSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDirection);

void main()
{
    const float GAMMA = 2.2; 
    vec3 norm = normalize(v_normal);
    vec3 viewDir = normalize(u_viewPosition - FragPos);

    vec3 result;

    if(u_isDirectionalLightEnabled)
    {
       result += calculateDirectionalLight(u_directionalLight, norm, viewDir);
    }
    
    if(u_isPointlLightEnabled)
    {
        for(int i = 0; i < NR_POINT_LIGHTS; i++)
        {
            result += calculatePointLight(u_pointLight[i], norm, FragPos, viewDir); 
        }
    }

    if(u_isSpotlLightEnabled)
    {
       result += calculateSpotLight(u_spotLight, norm, FragPos, viewDir); 
    }

    FragColor = vec4(result, 1.0); 
}

vec3 calculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDirection)
{ 
   vec3 lightDirection = normalize(light.direction); 

   // Ambient Lighting 
   vec3 ambient = light.ambient * vec3(texture(u_material.diffuse, v_TexCoord));

   // Diffuse Lighting 
   float diff = max(dot(normal, lightDirection), 0.0);
   vec3 diffuse = light.diffuse * diff * vec3(texture(u_material.diffuse, v_TexCoord));

   // Specular Lighting 
   vec3 halfwayDirection = normalize(lightDirection + viewDirection);
   float spec = pow(max(dot(normal, halfwayDirection), 0.0), u_material.shininess);
   vec3 specular = light.specular * spec * vec3(texture(u_material.specular, v_TexCoord));

    if(diff == 0.0)
    {
        spec = 0.0; 
    }
 
   return (ambient + diffuse + specular);
 }


vec3 calculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDirection)
{
    vec3 lightDirection = normalize(light.position - fragPos); 

    // Ambient Lighting
    vec3 ambient  = light.ambient * vec3(texture(u_material.diffuse, v_TexCoord));

    // Diffuse Lighting 
    float diff = max(dot(normal, lightDirection), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(u_material.diffuse, v_TexCoord));

    // Specular Lighting 
    vec3 halfwayDirection = normalize(lightDirection + viewDirection);
    float spec = pow(max(dot(normal, halfwayDirection), 0.0), u_material.shininess);
    vec3 specular = light.specular * spec * vec3(texture(u_material.specular, v_TexCoord));

    if(diff == 0.0)
    {
        spec = 0.0; 
    }

    // Attenuation 
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance)); 

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation; 

    return (ambient + diffuse + specular);
}

vec3 calculateSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDirection)
{
     vec3 lightDirection = normalize(light.position - fragPos); 

    // Ambient Lighting
    vec3 ambient  = light.ambient * vec3(texture(u_material.diffuse, v_TexCoord));

    // Diffuse Lighting 
    float diff = max(dot(normal, lightDirection), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(u_material.diffuse, v_TexCoord));

    // Specular Lighting 
    vec3 halfwayDirection = normalize(lightDirection + viewDirection);
    float spec = pow(max(dot(normal, halfwayDirection), 0.0), u_material.shininess);
    vec3 specular = light.specular * spec * vec3(texture(u_material.specular, v_TexCoord));

    if(diff == 0.0)
    {
        spec = 0.0; 
    }

    float theta = dot(lightDirection, normalize(-light.direction));
    float epsilon = light.innerCutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    // Attenuation 
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance)); 

    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity; 

    return (ambient + diffuse + specular);
}