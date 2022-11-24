#version 330

layout(location=0) in vec3 pos;
layout(location=1) in vec3 nor;
layout(location=2) in vec2 uv_in;
layout(location=3) in vec3 tg;

out vec3 normal;
out vec3 FragPos;
out vec2 uv;

out vec2 uvc;
out vec3 vnc;
out mat3 tbn;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main ()
{
	FragPos = vec3(modelMatrix * vec4(pos, 1.0));
    gl_Position = projectionMatrix * viewMatrix * vec4 (FragPos, 1.0);
	normal = mat3(transpose(inverse(modelMatrix))) * nor;
	mat3 normalMat = transpose(inverse(mat3(modelMatrix)));

	uv = uv_in;

	// Gram Schmidt orthonormalization
	vec3 _normal = normalize(nor);
    vec3 _tangent = normalize(tg);
    _tangent = normalize(_tangent - dot(_tangent, _normal) * _normal);
    vec3 _bitangent = cross(_normal, _tangent);

	//TBN Matrix
	vec3 T = normalize(vec3(normalMat * _tangent));
	vec3 B = normalize(vec3(normalMat * _bitangent));
	vec3 N = normalize(vec3(normalMat * _normal));
	tbn = mat3(T, B, N);

	uvc = uv_in;
	vnc = normalMat * nor;
}