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
	// Převod z lokálního do globalního souřadného systému - do homogenního
	FragPos = vec3(modelMatrix * vec4(pos, 1.0));
	
	// Projekční matice - perspektivní promítání, komolý jehlan (od -1 do 1)
	// View matice - kamera
    gl_Position = projectionMatrix * viewMatrix * vec4 (FragPos, 1.0);
	
	// Musíme toto přepočítat, aby se vytvoříla normála v globálním směru a nebyla jen v lokálním systému
	// Normálová matice - pomocí ní převedeme normálu do globálního systému
	// Transpose + inverse = přepočtení normály při neuniforním škálování
	normal = mat3(transpose(inverse(modelMatrix))) * nor;
}