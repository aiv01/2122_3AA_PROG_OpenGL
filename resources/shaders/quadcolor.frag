#version 450 core
out vec4 frag_color;

in vec4 vert_color_out;
//layout (location = 1) in vec4 PIPPO;

uniform vec4 base_color;
uniform vec4 base_color2;
//layout (location = 0) uniform vec4 base_color;
//layout (location = 1) uniform vec4 base_color2;


//uniform (location = 0) vec4 my_colors[2]; //LOC=0
//uniform (location = 1) vec4 base_color;   //LOC=2
//uniform vec4 base_color2;  //LOC=3

/*
struct mydata_t {
    float value1;
    float value2;
    float value3;
};

uniform mydata_t data[2];
*/

void main() {
    //frag_color = vec4(1.f, 1.f, 0.f, 1.f);
    //frag_color = vert_color_out;
    frag_color = vert_color_out * (base_color);
}
