#version 330 core
out vec4 FragColor;

in vec2 a_TexCoord;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

void main()
{
    FragColor = texture(texture_specular1, a_TexCoord) + texture(texture_diffuse1, a_TexCoord);
}