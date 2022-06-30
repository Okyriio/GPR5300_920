#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in mat4 aInstanceModel;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out mat3 Normal;
out vec3 FragPos;
out vec2 TexCoords;
out vec3 TangentLightPos;
out vec3 TangentViewPos  ;
out vec3 TangentFragPos  ;
out mat3 TBN;

void main()
{
    gl_Position = projection * view * aInstanceModel * vec4(aPos, 1.0);
    FragPos = vec3(aInstanceModel * vec4(aPos, 1.0));
    Normal = transpose(inverse(mat3(aInstanceModel)));
    TexCoords = aTexCoords;
  
    vec3 T = normalize(vec3(aInstanceModel * vec4(aTangent, 0.0)));
    vec3 N = normalize(vec3(aInstanceModel * vec4(aNormal, 0.0)));
    // re-orthogonalize T with respect to N
    T = normalize(T - dot(T, N) * N);
    // then retrieve perpendicular vector B with the cross product of T and N
    vec3 B = cross(N, T);


   TBN = mat3(T, B, N);
  
}       