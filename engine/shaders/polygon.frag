#version 330 core

in VertexOutput {
    vec4 color;
    vec2 vertices[8];
    vec2 position;
    flat int vertexCount;
    float radius;
    float thickness;
} fs_in;

out vec4 frag_color;

float cross2d(vec2 v, vec2 w) {
    return v.x * w.y - v.y * w.x;
}

float sdConvexPolygon(vec2 point, vec2[8] vertices, int vertexCount) {
    float minSqrDist = dot(point - vertices[0], point - vertices[0]);
    float side = -1.0f;
    int j = vertexCount - 1;
    for (int i = 0; i < vertexCount; ++i) {
        // distance to a polygon edge
        vec2 edge = vertices[i] - vertices[j];
        vec2 w = point - vertices[j];
        float projLength = dot(w, edge) / dot(edge, edge);
        vec2 distVec = w - edge * clamp(projLength, 0.0f, 1.0f);
        float sqrDist = dot(distVec, distVec);
        if (sqrDist < minSqrDist) {
            minSqrDist = sqrDist;
        }
        // if the cross product between w and edge is >= 0 then w is oriented clockwise relative to edge
        // the vertices are stored with a counter clockwise winding (CCW), so in this case the point is outside the
        // polygon
        if (cross2d(w, edge) >= 0.0f) {
            side = 1.0f;
        }
        j = i;
    }
    return side * sqrt(minSqrDist);
}

void main()
{
    float distanceToPolygon = sdConvexPolygon(fs_in.position, fs_in.vertices, fs_in.vertexCount);
    float d = abs(distanceToPolygon - fs_in.radius);
    float alpha = 1.0f - smoothstep(0.0f, fs_in.thickness, d);
    frag_color = vec4(fs_in.color.rgb, alpha);
}
