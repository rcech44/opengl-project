#version 330 core

out vec4 frag_colour;

struct Light {
    vec3 position;
    vec3 color;
    int type;
    float cut;
    float out_cut;
    vec3 direction;
};

#define LIGHTS 100

in vec3 normal;
in vec3 FragPos;

uniform vec3 objectColor;
uniform vec3 viewPos;
uniform int lightCount;

uniform Light lights[LIGHTS];

vec3 point_light(vec3 worldPos, vec3 normalVector, vec3 lightPosition, vec3 lightColor);
vec3 spot_light(vec3 worldPos, vec3 normalVector, vec3 lightPosition, vec3 lightDirection, vec3 lightColor, float cutoff, float outer_cutoff);

void main () 
{
    vec3 result;
	vec3 normalVector = normalize(normal);
	for (int i = 0; i < lightCount; i++)
	{
		vec3 lightPosition = lights[i].position;
        vec3 lightColor = lights[i].color;
        switch (lights[i].type)
        {
            case 1:
                result += point_light(FragPos, normalVector, lightPosition, lightColor);
                break;
            case 3:
                vec3 lightDirection = lights[i].direction;
                float cutoff = lights[i].cut;
                float outer_cutoff = lights[i].out_cut;
                result += spot_light(FragPos, normalVector, lightPosition, lightDirection, lightColor, cutoff, outer_cutoff);
                break;
            default:
                break;
        }
	}

	frag_colour = vec4(result, 1.0);
}

vec3 point_light(vec3 worldPos, vec3 normalVector, vec3 lightPosition, vec3 lightColor) 
{
    const float specularStrength = 0.4;
    const float ambientStrength = 0.15;

    vec3 ambient = ambientStrength * objectColor;

    float dist = length(lightPosition - worldPos);
    float attenuation = 1.0 / (1.0 + 0.09 * dist + 0.032 * (dist * dist));

    vec3 viewDir = normalize(viewPos - worldPos);
    vec3 lightDir = normalize(lightPosition - worldPos);
    vec3 reflectionDir = reflect(-lightDir, normalVector);

    float dot_product = dot(lightDir, normalVector);
    vec3 diffuse = max(dot_product, 0.0) * lightColor * attenuation;
    // vec4 diffuse = dot_product * vec4(0.385, 0.647, 0.812, 1.0);

    float specValue = pow(max(dot(viewDir, reflectionDir), 0.0), 16);
    vec3 spec = specularStrength * specValue * lightColor;
    if (dot_product < 0.0) {
        spec = vec3(0.0);
    }
    vec3 specular = attenuation * spec;

    return (diffuse + specular) * objectColor;
}

vec3 spot_light(vec3 worldPos, vec3 normalVector, vec3 lightPosition, vec3 lightDirection, vec3 lightColor, float cutoff, float outer_cutoff) 
{
    vec3 lightDir = normalize(lightPosition - worldPos);

    float theta = dot(lightDir, normalize(-lightDirection));

    const float specularStrength = 0.4;

    float dist = length(lightPosition - worldPos);
    float attenuation = 1.0 / (1.0 + 0.09 * dist + 0.032 * (dist * dist));

    vec3 viewDir = normalize(viewPos - worldPos);
    vec3 reflectionDir = reflect(-lightDir, normalVector);

    float epsilon = cutoff - outer_cutoff;
    float intensity = clamp((theta - outer_cutoff) / epsilon, 0.0, 1.0);

    float dot_product = dot(lightDir, normalVector);
    vec3 diffuse = max(dot_product, 0.0) * lightColor * attenuation;

    float specValue = pow(max(dot(viewDir, reflectionDir), 0.0), 16);
    vec3 spec = specularStrength * specValue * lightColor;
    if (dot_product < 0.0) {
        spec = vec3(0.0);
    }
    vec3 specular = attenuation * spec;

    diffuse *= intensity;
    specular *= intensity;

    return (diffuse + specular) * objectColor;

    

    // lmao

    //vec3 lightDir = normalize(light.position - fragPos);
    //// diffuse shading
    //float diff = max(dot(normal, lightDir), 0.0);
    //// specular shading
    //vec3 reflectDir = reflect(-lightDir, normal);
    //float spec = pow(max(dot(viewDir, reflectDir), 0.0), 16);
    //// attenuation
    //float distance = length(light.position - fragPos);
    //float attenuation = 1.0 / (1.0 + 0.09 * dist + 0.032 * (dist * dist));    
    //// spotlight intensity
    //float theta = dot(lightDir, normalize(-light.direction)); 
    //float epsilon = light.cutOff - light.outerCutOff;
    //float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    //// combine results
    //vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    //vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
    //vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    //ambient *= attenuation * intensity;
    //diffuse *= attenuation * intensity;
    //specular *= attenuation * intensity;
    //return (ambient + diffuse + specular);
}