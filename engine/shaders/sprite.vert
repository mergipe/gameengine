#version 330 core

layout(location = 0) in vec2 in_vertexPosition;
layout(location = 1) in vec2 in_vertexUv;
layout(location = 2) in mat4 in_instanceTransform;
layout(location = 6) in vec2 in_instanceUv;
layout(location = 7) in vec2 in_instanceSpriteSize;
layout(location = 8) in int in_instanceTextureIndex;
layout(location = 9) in vec4 in_instanceColor;

layout (std140) uniform Camera {
    mat4 u_viewProjection;
};

out VertexOutput {
    vec4 color;
    vec2 uv;
    flat int textureIndex;
} vs_out;

void main()
{
    vs_out.color = in_instanceColor;
    vs_out.uv = in_instanceUv + in_vertexUv * in_instanceSpriteSize;
    vs_out.textureIndex = in_instanceTextureIndex;
    gl_Position = u_viewProjection * in_instanceTransform * vec4(in_vertexPosition, 0.0, 1.0);
}
