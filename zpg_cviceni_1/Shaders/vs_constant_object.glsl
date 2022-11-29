#version 330

layout(location=0) in vec4 vp;
layout(location=1) in vec4 vc;

out vec4 color;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform vec3 objectColor;

void main ()
{
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4 (vp);
	color = vec4(objectColor, 1.0);
}