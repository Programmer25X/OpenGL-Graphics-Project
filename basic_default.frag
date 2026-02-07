#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;

uniform sampler2D texture1;
uniform vec3 u_lightColor; 


void main()
{
    vec4 texColor = texture(texture1, v_TexCoord);
    vec4 tempColor = vec4(u_lightColor, 1.0);
    color = texColor * tempColor; 
}