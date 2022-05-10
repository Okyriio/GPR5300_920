#version 300 es
precision highp float;

layout (location = 0) out vec4 fragColor;

uniform vec3 lightColor;
uniform vec3 objectColor;
uniform vec3 lightPos;  
in vec3 fragPos;  
in vec3 Normal;  

void main() {

    float ambientStrength = 0.4f;
    vec3 ambient = ambientStrength * lightColor;

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    vec3 result = (ambient + diffuse) * objectColor;
    fragColor = vec4(result, 1.0f);
}