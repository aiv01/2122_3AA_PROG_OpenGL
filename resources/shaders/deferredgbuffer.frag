#version 450 core
layout (location = 0) out vec3 gPosition; //GL_COLORATTACHMENT0
layout (location = 1) out vec3 gNormal;   //GL_COLORATTACHMENT1
layout (location = 2) out vec3 gDiffuse;  //GL_COLORATTACHMENT2

in vec2 vert_uv_out;
in vec3 world_pos_out;
in vec3 world_norm_out;

layout (binding = 0) uniform sampler2D diffuseText;

void main()
{    
    //just to show better coloring: normalize(worldPos); 
    //gPosition = normalize(worldPos); 
    
    gPosition = world_pos_out;  
    gNormal = normalize(world_norm_out);
    gDiffuse = texture(diffuseText, vert_uv_out).rgb;
}
