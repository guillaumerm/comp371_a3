#version 330 core

out vec4 color;

struct Light {
    vec3 position;
    vec3 color;
};

uniform Light lightsPartA[4];
uniform vec3 objectColor;
uniform vec3 viewPosition;

uniform float ambientCoefficient;
uniform float diffuseCoefficient;
uniform float specularCoefficient;

in vec3 normal;
in vec3 fragmentPosition;

vec3 calculateColor(Light light){
    // Ambient
    vec3 ambient = ambientCoefficient * light.color;

    // Diffuse
    vec3 lightDirection = normalize(light.position - fragmentPosition);
    float diffuseStrength = max(dot(normalize(normal), lightDirection), 0.0f);
    vec3 diffuse = diffuseCoefficient * diffuseStrength * light.color;

    // Specular
    vec3 viewDirection = normalize(viewPosition - fragmentPosition);
    vec3 reflectedLightDirection = reflect(-lightDirection, normalize(normal));
    float specularStrength = pow(max(dot(reflectedLightDirection, viewDirection), 0.0f), 32);
    vec3 specular = specularCoefficient * specularStrength * light.color;

    return (ambient + diffuse + specular);
}

void main()
{
    vec3 result;
    
    for(int i = 0; i < lightsPartA.length(); i++) {
        result += calculateColor(lightsPartA[i]);
    }

    color = vec4(result * objectColor, 1.0f);
}

