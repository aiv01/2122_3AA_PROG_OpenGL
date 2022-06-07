#version 450 core
layout (location = 0) in vec3 vert_pos;
layout (location = 1) in mat4 inst_mvp;
layout (location = 5) in vec2 vert_uv;

/*
layout (location = 1) in vec4 inst_mvp;
layout (location = 2) in vec4 inst_mvp;
layout (location = 3) in vec4 inst_mvp;
layout (location = 4) in vec4 inst_mvp;
*/


// uniform mat4 mvp;
out flat int instance_id;
out vec2 uv;

void main() {
    // gl_Position = mvp * vec4(vert_pos + gl_InstanceID / 10.f, 1.f);
    gl_Position = inst_mvp * vec4(vert_pos, 1.f);

    instance_id = gl_InstanceID;
    uv = vert_uv;
}