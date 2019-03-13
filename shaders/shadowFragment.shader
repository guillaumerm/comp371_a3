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

float ShadowCalculation(vec4 fragPosLightSpace)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // calculate bias (based on depth map resolution and slope)
    vec3 normal = normalize(normal);
    vec3 lightDir = normalize(lightPosition - fragmentPosition);
    float bias = max(0.025 * (1.0 - dot(normal, lightDir)), 0.0025);
    // check whether current frag pos is in shadow
    // float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
    // PCF
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;
    
    // keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    if(projCoords.z > 1.0)
        shadow = 0.0;
        
    return shadow;
}

void main()
{
    vec3 lightDirection = normalize(lightPosition - fragmentPosition);
    vec3 spotLightDirection = normalize(lightPosition - vec3(0,0,0));

    //float spotFactor = 0.0f;
    
    //float temp = dot(lightDirection, spotLightDirection);

    //if(temp >= cos(radians(12.5f))){
    //    spotFactor = pow(temp, 1);
    //}
    
    // Ambient
    vec3 ambient = ambientCoefficient * texture(diffuseTexture, texCoord).rgb;

    // Diffuse
    float diffuseStrength = max(dot(normalize(normal), lightDirection), 0.0f);
    vec3 diffuse = diffuseCoefficient * diffuseStrength * lightColor;

    // Specular
    vec3 viewDirection = normalize(viewPosition - fragmentPosition);
    vec3 reflectedLightDirection = reflect(-lightDirection, normalize(normal));
    float specularStrength = pow(max(dot(reflectedLightDirection, viewDirection), 0.0f), 32);
    vec3 specular = specularCoefficient * specularStrength * lightColor;
    
    // Shadow
    float shadow = ShadowCalculation(fragmentPositionLightSpace);
    vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * texture(diffuseTexture, texCoord).rgb;

    //vec3 lighting = (ambient + diffuse) * vec3(texture(diffuseTexture, texCoord));
    //color = vec4(lighting, 1.0f);
    color = vec4(lighting, 1.0f);
} 