#version 450 core
out vec4 frag_color;

in vec2 uv_out;

uniform sampler2D scene_tex;

void main() {
    frag_color = texture(scene_tex, uv_out);
    frag_color *= vec4(1.f, 0.f, 0.f, 1.f);
}
