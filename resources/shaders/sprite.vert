#version 330 core

layout(location = 0) in vec2 v_position;
layout(location = 1) in vec2 v_textureCoordinates;
out vec2 textureCoordinates;
uniform mat4 model;
uniform mat4 camera;
uniform mat4 projection;
uniform mat4 textureTransform;

void main()
{
    textureCoordinates = (textureTransform * vec4(v_textureCoordinates.x, 1 - v_textureCoordinates.y, 0.0, 1.0)).xy;
    gl_Position = projection * camera * model * vec4(v_position, 0.0, 1.0);
}
