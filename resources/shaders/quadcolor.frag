#version 450 core
out vec4 frag_color;

in vec4 vert_color_out;

uniform vec4 base_color;

void main() {
    //frag_color = vec4(1.f, 1.f, 0.f, 1.f);
    //frag_color = vert_color_out;
    frag_color = vert_color_out * (base_color);
}
