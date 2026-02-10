#version 330 core

layout(location = 0) out vec4 FragColor;

in vec2 v_TexCoord;
in vec3 v_normal;  
in vec3 FragPos; 

uniform vec3 u_lightPosition; 
uniform vec3 u_lightColor;
uniform float u_ambientStrength;
uniform vec3 u_objectColor; 
uniform vec3 u_viewPosition; 

void main()
{
    // Ambient Lighting 
    vec3 ambient = u_ambientStrength * u_lightColor;

    // Diffuse Lighting 
    vec3 norm = normalize(v_normal);
    vec3 lightDirection = normalize(u_lightPosition - FragPos);
    float diff = max(dot(norm, lightDirection), 0.0);
    vec3 diffuse = diff * u_lightColor;

    // Specular Lighting 
    float specularStrength = 0.5;
    vec3 viewDirection = normalize(u_viewPosition - FragPos);
    vec3 reflectionDirection = reflect(-lightDirection, norm);
    float spec = pow(max(dot(viewDirection, reflectionDirection), 0.0), 32);
    vec3 specular = specularStrength * spec * u_lightColor;

    vec3 result = (ambient + diffuse + specular) * u_objectColor;
    FragColor = vec4(result, 1.0);
}