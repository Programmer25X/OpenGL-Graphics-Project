#version 330 core

layout(location = 0) out vec4 FragColor;

in vec2 v_TexCoord;
in vec3 v_normal;  
in vec3 FragPos; 

uniform vec3 u_lightPosition; 
uniform vec3 u_lightColor;
uniform float u_ambientStrength;
uniform vec3 u_objectColor; 

void main()
{
    // Ambient Lighting 
    vec3 ambient = u_ambientStrength * u_lightColor;

    // Diffuse Lighting 
    vec3 norm = normalize(v_normal);
    vec3 lightDirection = normalize(u_lightPosition - FragPos);
    float diff = max(dot(norm, lightDirection), 0.0);
    vec3 diffuse = diff * u_lightColor;

    vec3 result = (ambient + diffuse) * u_objectColor;
    FragColor = vec4(result, 1.0);
}