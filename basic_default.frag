#version 330 core

layout(location = 0) out vec4 FragColor;

in vec2 v_TexCoord;
in vec3 aNormal;  
in vec3 FragPos; 

uniform sampler2D texture1;
uniform vec3 u_lightPosition; 
uniform vec3 u_lightColor;
uniform float u_ambientStrength;

void main()
{
    // Ambient Lighting 
    vec3 ambient = u_ambientStrength * u_lightColor;
    vec4 objectTexture = texture(texture1, v_TexCoord);
    vec3 texColor = objectTexture.rgb * objectTexture.a;

    // Diffuse Lighting 
    vec3 norm = normalize(aNormal);
    vec3 lightDirection = normalize(u_lightPosition - FragPos);
    float diff = max(dot(norm, lightDirection), 0.0);
    vec3 diffuse = diff * u_lightColor;

    vec3 result = (ambient + diffuse) * texColor ;
    FragColor = vec4(result, 1.0);
}