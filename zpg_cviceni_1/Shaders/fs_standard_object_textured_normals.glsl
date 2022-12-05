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
in vec3 vnc;
in vec2 uvc;

in mat3 tbn;
out vec4 frag_colour;

uniform vec3 viewPos;
uniform int lightCount;
uniform int flashlightEnabled;
uniform Light lights[LIGHTS];
uniform sampler2D textureUnitID_texture;
uniform sampler2D textureUnitID_normal;

vec3 point_light(vec3 worldPos, vec3 normalVector, vec3 lightPosition, vec3 lightColor, float lightStrength, vec3 color);
vec3 spot_light(vec3 worldPos, vec3 normalVector, vec3 lightPosition, vec3 lightDirection, vec3 lightColor, float cutoff, float outer_cutoff, float lightStrength, vec3 color);
vec3 directional_light(vec3 worldPos, vec3 normalVector, vec3 lightDirection, vec3 lightColor, float lightStrength, vec3 color);

void main () 
{
    vec3 result;

    // Normalize vrací stejný vektor akorát s délkou 1
	vec3 normalVector = normalize(normal);
	
    // Získání texelu na konkrétním místě
	vec4 tex = texture(textureUnitID_texture, uvc);
	
    // Získáni barev texelu
    vec3 color = vec3(tex.x, tex.y, tex.z);
	
	// Ambientni slozka
	vec3 ambient = vec3(0.02, 0.02, 0.02) * color;
	
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
			case 3:
				float cutoff = lights[i].cut;
				float outer_cutoff = lights[i].out_cut;
				result += spot_light(FragPos, normalVector, lightPosition, lightDirection, lightColor, cutoff, outer_cutoff, lightStrength, color);
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

	frag_colour = vec4(result + ambient, 1.0);
}

vec3 point_light(vec3 worldPos, vec3 normalVector, vec3 lightPosition, vec3 lightColor, float lightStrength, vec3 color) 
{
	vec3 ambient = vec3(0.1);
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

    // Získání normály v daném texelu a získání RGB složek
    vec3 encodedNormal  = texture(textureUnitID_normal, uvc).rgb;

    // Zajištění, že hodnoty budou od [-1, 1]
    encodedNormal = 2.0 * encodedNormal - 1.0; //RGB to vector

    // Nastavení intenzity normály (1,1,1)
    encodedNormal = normalize (encodedNormal*vec3(1,1,1)); //intensity

    // Získání normály z TBN matice
    vec3 _normal = normalize(tbn * encodedNormal) ;

    // Vypocet smeru odrazu
    vec3 reflectionDir = reflect(-lightDir, _normal);

    // toto nam vrati cosinus uhlu mezi dvema danymi vektory a max zajisti, ze neni zaporny
    float dot_product = dot(lightDir,  _normal);
    vec3 diffuse = abs(dot_product) * lightColor * attenuation;
    // vec4 diffuse = dot_product * vec4(0.385, 0.647, 0.812, 1.0);

    // Umocnění specular odrazu konstantou (změna intenzity)
    float specValue = pow(max(dot(viewDir, reflectionDir), 0.0), 16);
    vec3 spec = specularStrength * specValue * lightColor;

    // Pokud neni osvetleno, vratit 0
    if (dot_product < 0.0) {
        spec = vec3(0.1);
    }
    vec3 specular = attenuation * spec;

    return (diffuse + specular) * color * lightStrength;
}

vec3 spot_light(vec3 worldPos, vec3 normalVector, vec3 lightPosition, vec3 lightDirection, vec3 lightColor, float cutoff, float outer_cutoff, float lightStrength, vec3 color) 
{
    const float specularStrength = 0.4;
	
	// Odectenim pozice vrcholu od pozice svetla nam da smer svetla mezi pozici svetla a pozici fragmentu
    vec3 lightDirectionToObject = normalize(lightPosition - worldPos);

	// Získání normály v daném texelu a získání RGB složek
    vec3 encodedNormal  = texture(textureUnitID_normal, uvc).rgb;
	
	// Zajištění, že hodnoty budou od [-1, 1]
    encodedNormal = 2.0 * encodedNormal - 1.0; //RGB to vector
	
	// Nastavení intenzity normály (1,1,1)
    encodedNormal = normalize (encodedNormal*vec3(1,1,1)); //intensity
	
	// Získání normály z TBN matice
    vec3 _normal = normalize(tbn * encodedNormal) ;

    // Vzdalenost mezi svetlem a fragmentem
    float dist = length(lightPosition - worldPos);

	// toto nam vrati cosinus uhlu mezi vektorem smeru baterky (dopredu) a vektorem smeru k fragmentu
    float theta = dot(lightDirectionToObject, normalize(-lightDirection));

	// Vypocet utlumu - rovnice pro výpočet
    // Konstantní člen - často 1.0, zajišťuje, aby hodnota nebyla menší než jedna
    // Lineární člen - 0.09 * vzdálenost, linárně zmenšuje intenzitu
    // Kvadratický člen - 0.032 * pow(vzdálenost, 2), méně účinný při malých vzdálenostech, ale efektivní při větších vzdálenostech
    float attenuation = 1.0 / (1.0 + 0.05 * dist + 0.016 * (dist * dist));

	// Odectenim pozic fragmentu a kamery ziskame smer pohledu
    vec3 viewDir = normalize(viewPos - worldPos);
	
	// Vrati nam odrazovy smer (normaly z tangent space a smeru pohledu k fragmentu)
    vec3 reflectionDir = reflect(-lightDirectionToObject, _normal);

	// Rozdil orezovych uhlu
    float epsilon = cutoff - outer_cutoff;
	
	// Odecteni thety (cosinus uhlu mezi vektorem smeru baterky (dopredu) a vektorem smeru k fragmentu) a vnejsiho orezu a vydeleni rozdilem orezu, zajisteni hodnot mezi 0 a 1
    float intensity = clamp((theta - outer_cutoff) / epsilon, 0.0, 1.0);

	// toto nam vrati cosinus uhlu mezi novou normalou a smerem svetla k objektu
    float dot_product = dot(lightDirectionToObject, _normal);
	
	// difuzni slozka, max - zajisteni, ze neni nulove
    vec3 diffuse = abs(dot_product) * lightColor * attenuation;

	// Umocnění specular odrazu konstantou (změna intenzity)
    float specValue = pow(max(dot(viewDir, reflectionDir), 0.0), 16);
    vec3 spec = specularStrength * specValue * lightColor;
	
	// Pokud je objekt mimo svetlo, nastavit nulu
    if (dot_product < 0.0) {
        spec = vec3(0.1);
    }
    vec3 specular = attenuation * spec;

    diffuse *= intensity;
    specular *= intensity;

    return (diffuse + specular) * color * lightStrength;
}

vec3 directional_light(vec3 worldPos, vec3 normalVector, vec3 lightDirection, vec3 lightColor, float lightStrength, vec3 color)
{
    const float specularStrength = 0.4;

	// Získání normály v daném texelu a získání RGB složek
    vec3 encodedNormal  = texture(textureUnitID_normal, uvc).rgb;
	
	// Zajištění, že hodnoty budou od [-1, 1]
    encodedNormal = 2.0 * encodedNormal - 1.0; //RGB to vector
	
	// Nastavení intenzity normály (1,1,1)
    encodedNormal = normalize (encodedNormal*vec3(1,1,1)); //intensity
	
	// Získání normály z TBN matice
    vec3 _normal = normalize(tbn * encodedNormal) ;

	// Smer svetla
    vec3 lightDir = normalize(-lightDirection);
	
	// toto nam vrati cosinus uhlu mezi novou normalou a smerem svetla
    float dot_product = dot(lightDir, _normal);
	
	// difuzni slozka, max - zajisteni, ze neni nulove
    vec3 diffuse = abs(dot_product) * lightColor;

	// Smer k objektu - rozdil mezi pozici objektu a pozici kamery
    vec3 viewDir = normalize(viewPos - worldPos);
	
	// Smer reflektu - rozdil normaly a smeru svetla
    vec3 reflectionDir = reflect(-lightDir, _normal);

	// Umocnění specular odrazu konstantou (změna intenzity)
    float specValue = pow(max(dot(viewDir, reflectionDir), 0.0), 16);
    vec3 spec = specularStrength * specValue * lightColor;
	
	// Pokud je objekt mimo svetlo, nastavit nulu
    if (dot_product < 0.0) {
        spec = vec3(0.0);
    }

    return (diffuse + spec) * color * lightStrength;
}