#version 330

out vec4 frag_colour;

in vec3 normal;
in vec3 FragPos;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 objectColor;
uniform vec3 viewPos;

void main () 
{
	float ambientStrength = 0.15;
	float specularStrength = 1;
	vec3 ambient = ambientStrength * lightColor;
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(lightPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float diff = dot(norm, lightDir);
	vec3 diffuse = max(diff, 0.0) * lightColor;
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = specularStrength * spec * lightColor; 
	if ( diff < 0.0)
	{
		specular = vec3(0.0, 0.0, 0.0);
	}
	vec3 result = (ambient + diffuse + specular) * objectColor;
	frag_colour = vec4(result, 1.0);
}