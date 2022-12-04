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
out vec4 frag_colour;

uniform vec3 objectColor;
uniform vec3 viewPos;
uniform int lightCount;
uniform int flashlightEnabled;
uniform Light lights[LIGHTS];

vec3 point_light(vec3 worldPos, vec3 normalVector, vec3 lightPosition, vec3 lightColor, float lightStrength);
vec3 spot_light(vec3 worldPos, vec3 normalVector, vec3 lightPosition, vec3 lightDirection, vec3 lightColor, float cutoff, float outer_cutoff, float lightStrength);
vec3 directional_light(vec3 worldPos, vec3 normalVector, vec3 lightDirection, vec3 lightColor, float lightStrength);

void main () 
{
    vec3 result;
	vec3 normalVector = normalize(normal);
	
	// Ambientni slozka
	vec3 ambient = vec3(0.05, 0.05, 0.05) * objectColor;
	
	for (int i = 0; i < lightCount; i++)
	{
		vec3 lightPosition = lights[i].position;
        vec3 lightColor = lights[i].color;
        vec3 lightDirection = lights[i].direction;
        float lightStrength = lights[i].strength;
        switch (lights[i].type)
        {
            case 1:
                result += point_light(FragPos, normalVector, lightPosition, lightColor, lightStrength);
                break;
            case 2:
                result += directional_light(FragPos, normalVector, lightDirection, lightColor, lightStrength);
                break;
			case 3:
				float cutoff = lights[i].cut;
				float outer_cutoff = lights[i].out_cut;
				result += spot_light(FragPos, normalVector, lightPosition, lightDirection, lightColor, cutoff, outer_cutoff, lightStrength);
                break;
            case 4:
                if (flashlightEnabled == 1)
                {
                    float cutoff = lights[i].cut;
                    float outer_cutoff = lights[i].out_cut;
                    result += spot_light(FragPos, normalVector, lightPosition, lightDirection, lightColor, cutoff, outer_cutoff, lightStrength);
                }
                break;
            default:
                break;
        }
	}

	frag_colour = vec4(result + ambient, 1.0);
}

vec3 point_light(vec3 worldPos, vec3 normalVector, vec3 lightPosition, vec3 lightColor, float lightStrength) 
{
    const float specularStrength = 0.4;

	// Vzdalenost mezi svetlem a fragmentem
    float dist = length(lightPosition - worldPos);
	
	// Vypocet utlumu - rovnice pro výpočet
    // Konstantní člen - často 1.0, zajišťuje, aby hodnota nebyla menší než jedna
    // Lineární člen - 0.09 * vzdálenost, linárně zmenšuje intenzitu
    // Kvadratický člen - 0.032 * pow(vzdálenost, 2), méně účinný při malých vzdálenostech, ale efektivní při větších vzdálenostech
    float attenuation = 1.0 / (1.0 + 0.09 * dist + 0.032 * (dist * dist));

	// Odectenim pozic fragmentu a kamery ziskame smer pohledu
    vec3 viewDir = normalize(viewPos - worldPos);
	
	// Odectenim pozice vrcholu od pozice svetla nam da smer svetla mezi pozici svetla a pozici fragmentu
    vec3 lightDir = normalize(lightPosition - worldPos);
	
	// Vypocet smeru odrazu
    vec3 reflectionDir = reflect(-lightDir, normalVector);

	// Cosinus uhlu mezi dvema danymi vektory a max zajisti, ze neni zaporny
    float dot_product = dot(lightDir, normalVector);
    vec3 diffuse = max(dot_product, 0.0) * lightColor * attenuation;

	// Umocnění specular odrazu konstantou (změna intenzity)
    float specValue = pow(max(dot(viewDir, reflectionDir), 0.0), 16);
    vec3 spec = specularStrength * specValue * lightColor;
	
	// Pokud neni osvetleno, vratit 0
    if (dot_product < 0.0) {
        spec = vec3(0.0);
    }
    vec3 specular = attenuation * spec;

    return (diffuse + specular) * objectColor * lightStrength;
}

vec3 spot_light(vec3 worldPos, vec3 normalVector, vec3 lightPosition, vec3 lightDirection, vec3 lightColor, float cutoff, float outer_cutoff, float lightStrength) 
{
const float specularStrength = 0.4;
	
	// Odectenim pozice vrcholu od pozice svetla nam da smer svetla mezi pozici svetla a pozici fragmentu
    vec3 lightDirectionToObject = normalize(lightPosition - worldPos);

    // Vzdalenost mezi svetlem a fragmentem
    float dist = length(lightPosition - worldPos);

	// Cosinus uhlu mezi vektorem smeru baterky (dopredu) a vektorem smeru k fragmentu
    float theta = dot(lightDirectionToObject, normalize(-lightDirection));

	// Vypocet utlumu - rovnice pro výpočet
    // Konstantní člen - často 1.0, zajišťuje, aby hodnota nebyla menší než jedna
    // Lineární člen - 0.09 * vzdálenost, linárně zmenšuje intenzitu
    // Kvadratický člen - 0.032 * pow(vzdálenost, 2), méně účinný při malých vzdálenostech, ale efektivní při větších vzdálenostech
    float attenuation = 1.0 / (1.0 + 0.05 * dist + 0.016 * (dist * dist));

	// Odectenim pozic fragmentu a kamery ziskame smer pohledu
    vec3 viewDir = normalize(viewPos - worldPos);
	
	// Vrati nam odrazovy smer
    vec3 reflectionDir = reflect(-lightDirectionToObject, normalVector);

	// Rozdil orezovych uhlu
    float epsilon = cutoff - outer_cutoff;
	
	// Odecteni thety (cosinus uhlu mezi vektorem smeru baterky (dopredu) a vektorem smeru k fragmentu) a vnejsiho orezu a vydeleni rozdilem orezu, zajisteni hodnot mezi 0 a 1
    float intensity = clamp((theta - outer_cutoff) / epsilon, 0.0, 1.0);

	// Cosinus uhlu mezi normalou a smerem svetla k objektu
    float dot_product = dot(lightDirectionToObject, normalVector);
	
	// difuzni slozka, max - zajisteni, ze neni nulove
    vec3 diffuse = max(dot_product, 0.0) * lightColor * attenuation;

	// Umocnění specular odrazu konstantou (změna intenzity)
    float specValue = pow(max(dot(viewDir, reflectionDir), 0.0), 16);
    vec3 spec = specularStrength * specValue * lightColor;
	
	// Pokud je objekt mimo svetlo, nastavit nulu
    if (dot_product < 0.0) {
        spec = vec3(0.0);
    }
    vec3 specular = attenuation * spec;

    diffuse *= intensity;
    specular *= intensity;

    return (diffuse + specular) * objectColor * lightStrength;
}

vec3 directional_light(vec3 worldPos, vec3 normalVector, vec3 lightDirection, vec3 lightColor, float lightStrength)
{
    const float specularStrength = 0.4;

	// Smer svetla
    vec3 lightDir = normalize(-lightDirection);
	
	// Cosinus uhlu mezi smerem svetla a normalou fragmentu
    float dot_product = dot(lightDir, normalVector);
	
	// Difuzni slozka
    vec3 diffuse = max(dot_product, 0.0) * lightColor;

	// Smer pohledu k fragmentu od kamery
    vec3 viewDir = normalize(viewPos - worldPos);
	
	// Vypocet smeru odrazu
    vec3 reflectionDir = reflect(-lightDir, normalVector);

	// Zrcadlova slozka
    float specValue = pow(max(dot(viewDir, reflectionDir), 0.0), 16);
    vec3 spec = specularStrength * specValue * lightColor;
	
	// Pokud je objekt mimo svetlo, nastavit nulu
    if (dot_product < 0.0) {
        spec = vec3(0.0);
    }
 
    return (diffuse + spec) * objectColor * lightStrength;
}