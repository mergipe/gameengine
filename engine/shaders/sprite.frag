#version 330 core

in VertexOutput {
    vec4 color;
    vec2 uv;
    flat int textureIndex;
} fs_in;

uniform sampler2D tex_sprites[16];

out vec4 frag_color;

void main()
{
    frag_color = fs_in.color;
    switch (fs_in.textureIndex) {
        case 0: frag_color *= texture(tex_sprites[0], fs_in.uv); break;
        case 1: frag_color *= texture(tex_sprites[1], fs_in.uv); break;
        case 2: frag_color *= texture(tex_sprites[2], fs_in.uv); break;
        case 3: frag_color *= texture(tex_sprites[3], fs_in.uv); break;
        case 4: frag_color *= texture(tex_sprites[4], fs_in.uv); break;
        case 5: frag_color *= texture(tex_sprites[5], fs_in.uv); break;
        case 6: frag_color *= texture(tex_sprites[6], fs_in.uv); break;
        case 7: frag_color *= texture(tex_sprites[7], fs_in.uv); break;
        case 8: frag_color *= texture(tex_sprites[8], fs_in.uv); break;
        case 9: frag_color *= texture(tex_sprites[9], fs_in.uv); break;
        case 10: frag_color *= texture(tex_sprites[10], fs_in.uv); break;
        case 11: frag_color *= texture(tex_sprites[11], fs_in.uv); break;
        case 12: frag_color *= texture(tex_sprites[12], fs_in.uv); break;
        case 13: frag_color *= texture(tex_sprites[13], fs_in.uv); break;
        case 14: frag_color *= texture(tex_sprites[14], fs_in.uv); break;
        case 15: frag_color *= texture(tex_sprites[15], fs_in.uv); break;
    }
}
