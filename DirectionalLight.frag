#version 330 core

layout(location = 0) out vec4 FragColor;


// Material properties of a surface
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

#define NR_POINT_LIGHTS 4

in vec2 v_TexCoord;
in vec3 v_normal;  
in vec3 FragPos; 

uniform vec3 u_viewPosition;
uniform Material u_material;
uniform DirectionalLight u_directionalLight; 
uniform PointLight u_pointLight[NR_POINT_LIGHTS];

vec3 calculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDirection);
vec3 calculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDirection);

void main()
{
    vec3 norm = normalize(v_normal);
    vec3 viewDir = normalize(u_viewPosition - FragPos);

    vec3 result = calculateDirectionalLight(u_directionalLight, norm, viewDir);

    for(int i = 0; i < NR_POINT_LIGHTS; i++)
    {
       result += calculatePointLight(u_pointLight[i], norm, FragPos, viewDir); 
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
   vec3 reflectionDirection = reflect(-lightDirection, normal);
   float spec = pow(max(dot(viewDirection, reflectionDirection), 0.0), u_material.shininess);
   vec3 specular = light.specular * spec * vec3(texture(u_material.specular, v_TexCoord));
 
   return (ambient + diffuse + specular);
 }


vec3 calculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDirection)
{
    vec3 lightDirection = normalize(light.position - FragPos); 

    // Ambient Lighting
    vec3 ambient  = light.ambient * vec3(texture(u_material.diffuse, v_TexCoord));

    // Diffuse Lighting 
    float diff = max(dot(normal, lightDirection), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(u_material.diffuse, v_TexCoord));

    // Specular Lighting 
    vec3 reflectionDirection = reflect(-lightDirection, normal);
    float spec = pow(max(dot(viewDirection, reflectionDirection), 0.0), u_material.shininess);
    vec3 specular = light.specular * spec * vec3(texture(u_material.specular, v_TexCoord));

    // Attenuation 
    float distance = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance)); 

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation; 

    return (ambient + diffuse + specular);
}