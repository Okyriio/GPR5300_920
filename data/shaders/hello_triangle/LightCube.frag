#version 300 es
precision highp float;

layout (location = 0) out vec4 fragColor;

in vec3 Normal;
in vec3 fragPos;

uniform vec3 lightColor;
uniform vec3 viewPos;
uniform vec3 objectColor;
uniform vec3 lightPos;

void main() {

    //ambient
    float ambientStrength = 0.2;
    vec3 ambient = ambientStrength * lightColor;

    //Diffuse
    vec3 lightDir = normalize(lightPos - fragPos);
    vec3 norm = normalize(Normal);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    //specular
    float specularStrength = 0.1;
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float specularPow = 32.0f;
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), specularPow);
    vec3 specular = specularStrength * spec * lightColor;  

    vec3 result = (ambient + diffuse + specular) * objectColor;
    fragColor = vec4(result, 1.0);
}