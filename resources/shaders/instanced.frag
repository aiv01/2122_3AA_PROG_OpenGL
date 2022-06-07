#version 450 core
in flat int instance_id ;
in vec2 uv;

layout (binding = 0) uniform sampler2D smile_tex;
layout (binding = 1) uniform sampler2D wood_tex;

out vec4 frag_color;

void main() {
    // float scale_color = float(instance_id) / 10.f;
    // frag_color = vec4(scale_color, 0.f, 0.f, 1.f);

    // if (instance_id % 2 == 0) {
    //     frag_color = texture(smile_tex, uv);
    // } else {
    //     frag_color = texture(wood_tex, uv);
    // }
    // lerp in sostituzione dell'if sopra
    frag_color = mix(texture(smile_tex, uv), texture(wood_tex, uv), instance_id % 2 );
}
