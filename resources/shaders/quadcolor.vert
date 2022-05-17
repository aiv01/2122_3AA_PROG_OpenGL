#version 450 core
layout (location = 0) in vec3 vert_pos;
layout (location = 1) in vec3 vert_col;

out vec4 vert_color_out;
//layout (location = 1) out vec4 vert_color_out;


void main() {
    gl_Position = vec4(vert_pos, 1.f);
    vert_color_out = vec4(vert_col, 1.f);
}