#version 330

layout(location=0) in vec3 pos;

out vec3 fragmentLocalPosition;

uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main ()
{
	gl_Position = projectionMatrix * viewMatrix * vec4(pos, 1.0);
	fragmentLocalPosition = pos;
}