#version 450 core
layout (location = 0) in vec3 vert_pos;
layout (location = 1) in vec2 vert_uv;

out vec2 vert_uv_out;

vec3 perspective(vec3 pos)
{
    float aspect_ratio = 800.f / 600.f;
    float half_fov = 60.f / 2.f;
    float fov = tan(radians(half_fov));
    vec3 persp;
    persp.x = pos.x / (fov * aspect_ratio);
    persp.y = pos.y / fov;
    persp.z = 0.f;
    return persp;
}

void main() {
    vec3 fixed_vert = perspective(vert_pos);
    gl_Position = vec4(fixed_vert, 1.f);
    vert_uv_out = vert_uv;
    //vert_uv_out.y = 1 - vert_uv_out.y;
}