#version 330 core

in VertexOutput {
    vec4 color;
    vec2 position;
    float thickness;
} fs_in;

out vec4 frag_color;

float sdCircle(vec2 point, float radius) {
    return length(point) - radius;
}

void main()
{
    float distanceToCircle = abs(sdCircle(fs_in.position, 1.0f));
    // interpolates distanceToCircle between 0.0 and 1.0; if 0.0 <= distanceToCircle < thickness the fragment is in the
    // circle, so its alpha will be > 0.0
    float alpha = 1.0f - smoothstep(0.0f, fs_in.thickness, distanceToCircle);
    frag_color = vec4(fs_in.color.rgb, alpha);
}
