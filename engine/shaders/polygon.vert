#version 330 core

layout(location = 0) in vec2 in_vertexPosition;
layout(location = 1) in vec4 in_instanceTransform;
layout(location = 2) in vec4 in_instanceVertices12;
layout(location = 3) in vec4 in_instanceVertices34;
layout(location = 4) in vec4 in_instanceVertices56;
layout(location = 5) in vec4 in_instanceVertices78;
layout(location = 6) in int in_instanceVertexCount;
layout(location = 7) in float in_instanceRadius;
layout(location = 8) in vec4 in_instanceColor;

layout (std140) uniform Camera {
    mat4 u_viewProjection;
};

uniform float u_pixelScale;

out VertexOutput {
    vec4 color;
    vec2 vertices[8];
    vec2 position;
    flat int vertexCount;
    float radius;
    float thickness;
} vs_out;

void main()
{
    vs_out.color = in_instanceColor;
    vs_out.vertices[0] = in_instanceVertices12.xy;
    vs_out.vertices[1] = in_instanceVertices12.zw;
    vs_out.vertices[2] = in_instanceVertices34.xy;
    vs_out.vertices[3] = in_instanceVertices34.zw;
    vs_out.vertices[4] = in_instanceVertices56.xy;
    vs_out.vertices[5] = in_instanceVertices56.zw;
    vs_out.vertices[6] = in_instanceVertices78.xy;
    vs_out.vertices[7] = in_instanceVertices78.zw;
    vs_out.position = in_vertexPosition;
    vs_out.vertexCount = in_instanceVertexCount;
    vs_out.radius = in_instanceRadius;

    vec2 aabbLowerLeft = vs_out.vertices[0];
    vec2 aabbUpperRight = vs_out.vertices[0];
    for (int i = 1; i < in_instanceVertexCount; ++i) {
        aabbLowerLeft = min(aabbLowerLeft, vs_out.vertices[i]);
        aabbUpperRight = max(aabbUpperRight, vs_out.vertices[i]);
    }
    vec2 aabbCenter = (aabbLowerLeft + aabbUpperRight) * 0.5f;
    vec2 aabbSize = aabbUpperRight - aabbLowerLeft;
    float aabbMaxSize = max(aabbSize.x, aabbSize.y);

    // scale and translate polygon points so they fit in 2x2 quad
    float scale = aabbMaxSize * 0.5f + vs_out.radius;
    for (int i = 0; i < vs_out.vertexCount; ++i) {
        vs_out.vertices[i] = (vs_out.vertices[i] - aabbCenter) / scale;
    }
    vs_out.radius = vs_out.radius / scale;
    vs_out.thickness = 3000.0f / (u_pixelScale * scale);

    float x = in_instanceTransform.x;
    float y = in_instanceTransform.y;
    float c = in_instanceTransform.z;
    float s = in_instanceTransform.w;
    // scale quad to fit polygon
    vec2 position = scale * in_vertexPosition + aabbCenter;
    // translate and rotate quad
    position = vec2(c * position.x - s * position.y + x, s * position.x + c * position.y + y);
    gl_Position = u_viewProjection * vec4(position, 0.0f, 1.0f);
}
