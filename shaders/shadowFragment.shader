#version 330 core

in vec3 normal;
in vec3 fragmentPosition;
in vec2 texCoord;
in vec4 fragmentPositionLightSpace;

uniform sampler2D diffuseTexture;
uniform sampler2D shadowMap;

uniform vec3 lightPosition;
uniform vec3 lightColor;
uniform vec3 viewPosition;

uniform float ambientCoefficient;
uniform float diffuseCoefficient;
uniform float specularCoefficient;

out vec4 color;

// Calculates the shadow value at the given fragment position relative to the light source
float calculateShadow(vec4 fragmentPositionLightSpace, vec3 lightDirection)
{
    // perform perspective divide
    vec3 projectedCoordinatess = fragmentPositionLightSpace.xyz / fragmentPositionLightSpace.w;
    // transform to [0,1] range
    projectedCoordinatess = projectedCoordinatess * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projectedCoordinatess.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projectedCoordinatess.z;
    // calculate bias (based on depth map resolution and slope)
    vec3 normal = normalize(normal);
    float bias = max(0.0025 * (1.0 - dot(normal, lightDirection)), 0.00025);
    // check whether current fragment position is in shadow
    if(currentDepth - bias > closestDepth){
        return 1.0;
    }else{
        return 0.0;
    }
}

void main()
{
    vec3 lightDirection = normalize(lightPosition - fragmentPosition);
    vec3 spotLightDirection = normalize(lightPosition - vec3(0,0,0));

    //float spotFactor = 0.0f;
    
    float theta = dot(lightDirection, spotLightDirection);

    // Ambient
    vec3 ambient = ambientCoefficient * texture(diffuseTexture, texCoord).rgb;
    vec3 lighting;

    // Verifies that the fragment is within the cutoff range of the spotlight
    if(theta >= cos(radians(20.0f))){
        // Diffuse
        float diffuseStrength = max(dot(normalize(normal), lightDirection), 0.0f);
        vec3 diffuse = diffuseCoefficient * diffuseStrength * lightColor;

        // Specular
        vec3 viewDirection = normalize(viewPosition - fragmentPosition);
        vec3 reflectedLightDirection = reflect(-lightDirection, normalize(normal));
        float specularStrength = pow(max(dot(reflectedLightDirection, viewDirection), 0.0f), 32);
        vec3 specular = specularCoefficient * specularStrength * lightColor;
        
        // Shadow
        float shadow = calculateShadow(fragmentPositionLightSpace, lightDirection);
        lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * texture(diffuseTexture, texCoord).rgb;
    } else {
        lighting = ambient * texture(diffuseTexture, texCoord).rgb;
    }

    color = vec4(lighting, 1.0f);
} 