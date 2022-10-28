#version 330

layout(location=0) in vec3 pos;
layout(location=1) in vec3 nor;

out vec3 normal;
out vec3 FragPos;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main ()
{
	FragPos = vec3(modelMatrix * vec4(pos, 1.0));
    gl_Position = projectionMatrix * viewMatrix * vec4 (FragPos, 1.0);
	normal = mat3(transpose(inverse(modelMatrix))) * nor;
}