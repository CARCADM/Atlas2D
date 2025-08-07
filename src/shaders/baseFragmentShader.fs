#version 330 core

out vec4 fragColor;

in vec2 texCoord;

uniform sampler2D TEXTURE0;

void main()
{
    fragColor = texture(TEXTURE0, texCoord);
}