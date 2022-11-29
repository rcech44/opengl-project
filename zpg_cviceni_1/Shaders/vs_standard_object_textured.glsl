#version 330

layout(location=0) in vec3 pos;
layout(location=1) in vec3 nor;
layout(location=2) in vec2 uv_in;

out vec3 normal;
out vec3 FragPos;
out vec2 uv;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main ()
{
	// Převod z lokálního do globalního souřadného systému - do homogenního
	FragPos = vec3(modelMatrix * vec4(pos, 1.0));
	
	// Projekční matice - perspektivní promítání, komolý jehlan (od -1 do 1)
	// View matice - kamera
    gl_Position = projectionMatrix * viewMatrix * vec4 (FragPos, 1.0);
	
	// Normálová matice - pomocí ní převedeme normálu do globálního systému
	normal = mat3(transpose(inverse(modelMatrix))) * nor;
	
	// Texturové souřadnice
	uv = uv_in;
}