#version 330 core

layout(location = 0) in vec2 in_vertexPosition;
layout(location = 1) in vec2 in_instancePosition;
layout(location = 2) in float in_instanceRadius;
layout(location = 3) in vec4 in_instanceColor;

layout (std140) uniform Camera {
    mat4 u_viewProjection;
};

uniform float u_pixelScale;

out VertexOutput {
    vec4 color;
    vec2 position;
    float thickness;
} vs_out;

void main()
{
    vs_out.color = in_instanceColor;
    vs_out.position = in_vertexPosition;
    vs_out.thickness = 3000.0f / (u_pixelScale * in_instanceRadius);
    vec2 position = in_instanceRadius * in_vertexPosition + in_instancePosition;
    gl_Position = u_viewProjection * vec4(position, 0.0, 1.0);
}
