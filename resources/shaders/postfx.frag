#version 450 core
out vec4 frag_color;

layout (binding = 0) uniform sampler2D wood_tex;
in vec2 uv_out;

void main() {
    vec4 wood_texel = texture(wood_tex, uv_out);
    frag_color = wood_texel;
}
