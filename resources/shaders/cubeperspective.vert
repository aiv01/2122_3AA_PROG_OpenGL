#version 450 core
layout (location = 0) in vec3 vert_pos;
layout (location = 1) in vec2 vert_uv;

out vec2 vert_uv_out;

vec3 perspective(vec3 pos)
{
    float aspect_ratio = 800.f / 600.f;
    float half_fov = 60.f / 2.f;
    float fov = tan(radians(half_fov));
    
    float near = 0.1f;
    float far = 1000.f;
    float range = far - near;

    vec3 persp;
    persp.x = pos.x / -pos.z;
    persp.y = pos.y / -pos.z;
    persp.z = ((pos.z - near) / range) * 2 + 1;
    
    persp.x = persp.x / (fov * aspect_ratio);
    persp.y = persp.y / fov;

    return persp;
}

vec3 scale(vec3 vec, float scale)
{
    return vec * scale;
}

void main() {
    vec3 vert_mod = vert_pos;
    vert_mod = scale(vert_mod, 100);
    
    vert_mod.z -= 400.f;
    vec3 fixed_vert = perspective(vert_mod);
    gl_Position = vec4(fixed_vert, 1.f);
    vert_uv_out = vert_uv;
    //vert_uv_out.y = 1 - vert_uv_out.y;
}