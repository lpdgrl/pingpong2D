#version 460 core

out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D textureball;

void main()
{
    FragColor = texture(textureball, TexCoord);
}
