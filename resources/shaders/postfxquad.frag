#version 450 core
out vec4 frag_color;

in vec2 uv_out;

uniform sampler2D scene_tex;
uniform float time;

void main() {
    vec4 color = texture(scene_tex, uv_out);
    
    /* Grayscale standard */
    /*
    float average = (color.r + color.g + color.b) / 3.f;
    frag_color = vec4(average, average, average, 1.f);   
    */

    /* Grayscale weighted */
    /*
    float weighted = ( color.r * 0.3f + color.g * 0.59f + color.b * 0.11f);
    frag_color = vec4(weighted, weighted, weighted, 1.f); 
    */

    /* Negative */
    /*
    frag_color = 1.f - color;
    */

    vec2 uv = uv_out;
    uv.x = uv.x + sin(uv.y * 20.f + time) / 100.f;
    frag_color = texture(scene_tex, uv);

    //frag_color = texture(scene_tex, uv_out);
    //frag_color *= vec4(1.f, 0.f, 0.f, 1.f);
}
