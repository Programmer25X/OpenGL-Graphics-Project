#version 330 core

layout(location = 0) out vec4 FragColor;


// Material properties of a surface
struct Material
{
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission; 
    float shininess;
};

struct Light
{
    vec3 position; 
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

in vec2 v_TexCoord;
in vec3 v_normal;  
in vec3 FragPos; 

uniform vec3 u_viewPosition;
uniform Material u_material;
uniform Light u_light;

void main()
{

    vec3 lightDirection = normalize(u_light.direction); 

    // Ambient Lighting 
    vec3 ambient = u_light.ambient * vec3(texture(u_material.diffuse, v_TexCoord));

    // Diffuse Lighting 
    vec3 norm = normalize(v_normal);
    float diff = max(dot(norm, lightDirection), 0.0);
    vec3 diffuse = u_light.diffuse * diff * vec3(texture(u_material.diffuse, v_TexCoord));

    // Specular Lighting 
    vec3 viewDirection = normalize(u_viewPosition - FragPos);
    vec3 reflectionDirection = reflect(-lightDirection, norm);
    float spec = pow(max(dot(viewDirection, reflectionDirection), 0.0), u_material.shininess);
    vec3 specular = u_light.specular * spec * vec3(texture(u_material.specular, v_TexCoord));

    vec3 emission = vec3(texture(u_material.emission, v_TexCoord));

    vec3 result = ambient + diffuse + specular + emission;
    FragColor = vec4(result, 1.0);
}
