#version 300 es
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;


out vec2 texCoord;
 
void main()
{
	gl_Position = vec4(aPos, 0.5);
	texCoord = aTexCoord;
}