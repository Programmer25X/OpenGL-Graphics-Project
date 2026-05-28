#version 330 core

layout(location = 0) out vec4 FragColor;


// Material properties of a surface
struct Material
{
    sampler2D diffuse;
    vec3 specular;
    float shininess;
};

struct Light
{
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec2 v_TexCoord;
in vec3 v_normal;  
in vec3 FragPos; 

uniform vec3 u_viewPosition;
uniform Material u_material;
uniform Light u_light;

void main()
{
    // Ambient Lighting 
    vec3 ambient = u_light.ambient * texture(u_material.diffuse, v_TexCoord).rgb;

    // Diffuse Lighting 
    vec3 norm = normalize(v_normal);
    vec3 lightDirection = normalize(u_light.position - FragPos);
    float diff = max(dot(norm, lightDirection), 0.0);
    vec3 diffuse = u_light.diffuse * diff * texture(u_material.diffuse, v_TexCoord).rgb;

    // Specular Lighting 
    vec3 viewDirection = normalize(u_viewPosition - FragPos);
    vec3 reflectionDirection = reflect(-lightDirection, norm);
    float spec = pow(max(dot(viewDirection, reflectionDirection), 0.0), u_material.shininess);
    vec3 specular = u_light.specular * (spec * u_material.specular);

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}