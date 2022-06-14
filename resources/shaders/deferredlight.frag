#version 450 core
out vec4 fragColor;

in vec2 uv_out;

layout (binding = 0) uniform sampler2D gPosition;
layout (binding = 1) uniform sampler2D gNormal;
layout (binding = 2) uniform sampler2D gDiffuse;

uniform vec3 cameraPos;
uniform vec3 pointLightPos[3];

vec3 phong(vec3 pointLightPos) {
    vec2 vert_uv = uv_out;
    //Sample values from GBuffer
    vec3 worldPos = texture(gPosition, vert_uv).rgb;
    vec3 worldNorm = texture(gNormal, vert_uv).rgb;
    vec3 meshColor = texture(gDiffuse, vert_uv).xyz;

    //Than phong algoritm as usual
    vec3 lightColor = vec3(1.f, 1.f, 1.f);
    vec3 specularColor = vec3(1.f, 1.f, 1.f);
    float specularShineness = 40.f;

    vec3 norm = normalize(worldNorm);
    vec3 lightDir = normalize(pointLightPos - worldPos); 

    //Ambient Effect
    float ambientFactor = 0.2f;
    vec3 ambient = lightColor * meshColor * ambientFactor;

    //Diffuse Effect
    float diffuseStrength = max(dot(norm, lightDir), 0); //max to avoid negative value of dot prod
    vec3 diffuse = lightColor * meshColor * diffuseStrength;
    //vec3 diffuse = mix(lightColor, meshColor, 0.f) * diffuseStrength;

    //Specular Effect
    vec3 eyeDir = normalize(cameraPos - worldPos);
    vec3 reflectDir = reflect(-lightDir, norm);  //Light direction reflected around the normal vector.
    float specularStrength = max(dot(eyeDir, reflectDir), 0);
    float specularFactor = pow(specularStrength, specularShineness);
    //vec3 specular = light.specular * material.specular * specularFactor;  
    vec3 specular = specularColor * specularFactor;  

    vec3 phong = ambient + diffuse + specular;


    //Light parameters
    //https://learnopengl.com/Lighting/Light-casters
    float k_constant   = 1.f;
    float k_linear    = 0.07f;
    float k_quadratic = 0.017f; 
    float distance   = length(pointLightPos - worldPos);

    float attenuation = 1.f / ( k_constant + 
                                k_linear * distance + 
    		                    k_quadratic * (distance * distance));    

    phong *= attenuation;
    return phong;
}

void main() {
    //TESTING
    //fragColor = vec4(uv_out, 0.f, 1.f);
    //fragColor = vec4(texture(gDiffuse, uv_out).rgb, 1.f);
    //fragColor = normalize(vec4(texture(gNormal, uv_out).rgb, 1.f));
    //fragColor = normalize(vec4(texture(gPosition, uv_out).rgb, 1.f));

    vec3 phong = vec3(0.f);

    //For each light compute phong for the current fragment   
    for(int i=0; i < 3; i++) {
        phong += phong(pointLightPos[i]);
    }
    fragColor = vec4(phong, 1.f);
}