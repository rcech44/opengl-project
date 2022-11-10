#version 330 core

struct Light {
    vec3 position;
    vec3 color;
    int type;
    float cut;
    float out_cut;
    vec3 direction;
    float strength;
};

#define LIGHTS 100

in vec3 normal;
in vec3 FragPos;        
in vec2 uv;
out vec4 frag_colour;

// uniform vec3 objectColor;
uniform vec3 viewPos;
uniform int lightCount;
uniform int flashlightEnabled;
uniform Light lights[LIGHTS];
uniform sampler2D textureUnitID;

vec3 point_light(vec3 worldPos, vec3 normalVector, vec3 lightPosition, vec3 lightColor, float lightStrength, vec3 color);
vec3 spot_light(vec3 worldPos, vec3 normalVector, vec3 lightPosition, vec3 lightDirection, vec3 lightColor, float cutoff, float outer_cutoff, float lightStrength, vec3 color);
vec3 directional_light(vec3 worldPos, vec3 normalVector, vec3 lightDirection, vec3 lightColor, float lightStrength, vec3 color);

void main () 
{
    vec3 result;
	vec3 normalVector = normalize(normal);
	
	vec4 tex = texture(textureUnitID, uv);
    vec3 color = vec3(tex.x, tex.y, tex.z);
	
	for (int i = 0; i < lightCount; i++)
	{
		vec3 lightPosition = lights[i].position;
        vec3 lightColor = lights[i].color;
        vec3 lightDirection = lights[i].direction;
        float lightStrength = lights[i].strength;
        switch (lights[i].type)
        {
            case 1:
                result += point_light(FragPos, normalVector, lightPosition, lightColor, lightStrength, color);
                break;
            case 2:
                result += directional_light(FragPos, normalVector, lightDirection, lightColor, lightStrength, color);
                break;
            case 4:
                if (flashlightEnabled == 1)
                {
                    float cutoff = lights[i].cut;
                    float outer_cutoff = lights[i].out_cut;
                    result += spot_light(FragPos, normalVector, lightPosition, lightDirection, lightColor, cutoff, outer_cutoff, lightStrength, color);
                }
                break;
            default:
                break;
        }
	}

	frag_colour = vec4(result, 1.0);
}

vec3 point_light(vec3 worldPos, vec3 normalVector, vec3 lightPosition, vec3 lightColor, float lightStrength, vec3 color) 
{
    const float specularStrength = 0.4;

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

    return (diffuse + specular) * color * lightStrength;
}

vec3 spot_light(vec3 worldPos, vec3 normalVector, vec3 lightPosition, vec3 lightDirection, vec3 lightColor, float cutoff, float outer_cutoff, float lightStrength, vec3 color) 
{
    vec3 lightDir = normalize(lightPosition - worldPos);

    // diffuse
    float dist = length(lightPosition - worldPos);

    float theta = dot(lightDir, normalize(-lightDirection));

    const float specularStrength = 0.4;

    float attenuation = 1.0 / (1.0 + 0.05 * dist + 0.016 * (dist * dist));

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

    return (diffuse + specular) * color * lightStrength;
}

vec3 directional_light(vec3 worldPos, vec3 normalVector, vec3 lightDirection, vec3 lightColor, float lightStrength, vec3 color)
{
    const float specularStrength = 0.4;

    vec3 lightDir = normalize(-lightDirection);
    float dot_product = dot(lightDir, normalVector);
    vec3 diffuse = max(dot_product, 0.0) * lightColor;

    vec3 viewDir = normalize(viewPos - worldPos);
    vec3 reflectionDir = reflect(-lightDir, normalVector);

    float specValue = pow(max(dot(viewDir, reflectionDir), 0.0), 16);
    vec3 spec = specularStrength * specValue * lightColor;
    if (dot_product < 0.0) {
        spec = vec3(0.0);
    }

    return (diffuse + spec) * color * lightStrength;
}