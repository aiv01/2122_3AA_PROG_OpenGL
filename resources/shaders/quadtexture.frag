#version 450 core
out vec4 frag_color;

in vec2 vert_uv_out;

layout (binding = 0) uniform sampler2D smile_tex;
layout (binding = 1) uniform sampler2D wood_tex;

//uniform sampler2D smile_tex;
//uniform sampler2D wood_tex;


void main() {
    vec4 smile_texel = texture(smile_tex, vert_uv_out);
    vec4 wood_texel = texture(wood_tex, vert_uv_out);
    frag_color = mix(smile_texel, wood_texel, 0.5f);
    //frag_color = smile_texel * wood_texel;
}
