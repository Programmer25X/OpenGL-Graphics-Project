#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;

uniform sampler2D texture1;
// uniform sampler2D texture2;


void main()
{
    // vec4 texColor = mix(texture(texture1, v_TexCoord), texture(texture2, v_TexCoord), 0.2);
    vec4 texColor = texture(texture1, v_TexCoord);
    color = texColor;
};