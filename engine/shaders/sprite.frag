#version 330 core

in vec2 textureCoordinates;
out vec4 fragment_color;
uniform sampler2D textureSampler;
uniform vec3 color;

void main()
{
    fragment_color = vec4(color, 1.0) * texture(textureSampler, textureCoordinates);
}
