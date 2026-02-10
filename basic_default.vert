#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;

out vec2 v_TexCoord;
out vec3 FragPos;
out vec3 v_normal;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

void main()
{
    FragPos = vec3(u_model * vec4(aPos, 1.0));
    v_normal = aNormal; 
    v_TexCoord = aTexCoord;
    gl_Position = u_projection * u_view * vec4(FragPos, 1.0);
}