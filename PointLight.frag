

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

}