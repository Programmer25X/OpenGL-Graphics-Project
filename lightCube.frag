#version 330 core
out vec4 FragColor;

uniform vec3 u_lightSourceColour;

void main()
{
    FragColor = vec4(u_lightSourceColour, 1.0); // Sets all 4 vector values to 1.0
}