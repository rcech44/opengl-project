#version 330

layout(location=0) in vec3 pos;
layout(location=1) in vec3 nor;
layout(location=2) in vec2 uv_in;
layout(location=3) in vec3 tg;

out vec3 normal;
out vec3 FragPos;

out vec2 uvc;
out vec3 vnc;
out mat3 tbn;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main ()
{
	// Převod z lokálního do globalního souřadného systému - do homogenního
	FragPos = vec3(modelMatrix * vec4(pos, 1.0));

	// Projekční matice - perspektivní promítání
	// View matice - kamera
    gl_Position = projectionMatrix * viewMatrix * vec4 (FragPos, 1.0);

	// Musíme toto přepočítat, aby se vytvoříla normála v globálním směru a nebyla jen v lokálním systému
	// Normálová matice - pomocí ní převedeme normálu do globálního systému
	// Transpose + inverse = přepočtení normály při neuniforním škálování
	normal = mat3(transpose(inverse(modelMatrix))) * nor;
	mat3 normalMat = transpose(inverse(mat3(modelMatrix)));

	// Gram Schmidt orthonormalization - zajisteni, ze tangenta a bitangenta budou kolme (nemusi byt)
	vec3 _normal = normalize(nor);
    vec3 _tangent = normalize(tg);

	// Dot product - součin vektorů, udává vychýlení dvou vektorů v rozmezí (-1, 1), kdy -1 = jsou opačně, 1 = jsou ve stejném směru, 0 = kolmé
    _tangent = normalize(_tangent - dot(_tangent, _normal) * _normal);
	
	// Cross product - nalezeni vektoru, ktery je kolmy na tangetu a normalu = bitangenta
    vec3 _bitangent = cross(_normal, _tangent);

	// TBN Matrix (Pomoci TBN matice muzeme transformovat normaly z textury (Tangent Space) do Model Space = zarotuje)
	vec3 T = normalize(vec3(normalMat * _tangent));
	vec3 B = normalize(vec3(normalMat * _bitangent));
	vec3 N = normalize(vec3(normalMat * _normal));
	tbn = mat3(T, B, N);

	// Vratit
	uvc = uv_in;
	vnc = normalMat * nor;
}