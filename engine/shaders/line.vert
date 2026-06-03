#version 330 core

layout(location = 0) in vec2 in_position;
layout(location = 1) in vec4 in_color;

layout (std140) uniform Camera {
    mat4 u_viewProjection;
};

out VertexOutput {
    vec4 color;
} vs_out;

void main()
{
    vs_out.color = in_color;
    gl_Position = u_viewProjection * vec4(in_position, 0.0, 1.0);
}
