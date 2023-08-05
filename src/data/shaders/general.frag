#version 330 core

struct Material
{
    sampler2D diffuse;
    sampler2D specular;

    float shininess;
};

struct LightProps
{
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct DirectLight
{
    vec3 direction;

    LightProps props;
};

struct PointLight
{
    float constant;
    float linear;
    float quadratic;

    LightProps props;
};

struct Spotlight
{
    vec3 direction;

    float innerCutoffCos;
    float outerCutoffCos;

    PointLight pointLight;
};

#define POINT_LIGHT_COUNT 1
#define SPOT_LIGHT_COUNT 1
#define DIRECT_LIGHT_COUNT 1

//in vec3 a_OurColor;
in vec2 a_TexCoord;
in vec3 a_NormalVec;
in vec3 a_FragPos;

uniform Material u_Material;
uniform DirectLight u_DirectLight[DIRECT_LIGHT_COUNT];
uniform PointLight u_PointLight[POINT_LIGHT_COUNT];
uniform Spotlight u_Spotlight[SPOT_LIGHT_COUNT];

uniform vec3 u_EmissionLight;
uniform sampler2D u_Emission;

uniform float u_TexMixValue;
uniform vec3 u_ViewerPos;

out vec4 fragColor;

float calcAttenuation(PointLight pointLight)
{
    float distance = length(pointLight.props.position - a_FragPos);
    float divisor = pointLight.constant + pointLight.linear * distance + pointLight.quadratic * pow(distance, 2.0);
    // Return 0 attenuation if divisor is 0
    return divisor > 0 ? 1.0 / divisor : 0.0;
}

float calcSpotlightIntensity(Spotlight spotlight, vec3 lightDir)
{
    float thetaCos = dot(lightDir, normalize(-spotlight.direction));
    float epsilon = spotlight.innerCutoffCos - spotlight.outerCutoffCos;
    float spotlightIntensity = clamp((thetaCos - spotlight.outerCutoffCos) / epsilon, 0.0, 1.0);
    return spotlightIntensity;
}

vec3 calcDirLight(DirectLight light, vec3 normal, vec3 viewDir, vec3 diffuseMap, vec3 specularMap)
{
    vec3 lightDir = normalize(-light.direction);
    float diffusionCosAngle = max(dot(normal, lightDir), 0.0);
    // Reflect the *negated* light vector w.r.t to the normal vector
    // We negate the light vector since we want the reflection to point
    vec3 reflectDir = reflect(-lightDir, normal);
    float specularMultiplier = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.shininess);
    // combine results
    vec3 ambient = light.props.ambient * diffuseMap;
    vec3 diffuse = light.props.diffuse * diffusionCosAngle * diffuseMap;
    vec3 specular = light.props.specular * specularMultiplier * specularMap;
    return (ambient + diffuse + specular);
}

vec3 calcPointLight(PointLight light, vec3 normal, vec3 viewDir, vec3 diffuseMap, vec3 specularMap)
{
    vec3 lightDir = normalize(light.props.position - a_FragPos);
    // Diffuse shading
    float diffusionAngle = max(dot(normal, lightDir), 0.0);
    // Reflect the *negated* light vector w.r.t to the normal vector
    // We negate the light vector since we want the reflection to point
    vec3 reflectDir = reflect(-lightDir, normal);
    float specularMultiplier = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.shininess);
    // Attenuation
    float attenuation = calcAttenuation(light);
    // Combine results
    vec3 ambient = light.props.ambient * diffuseMap * attenuation;
    vec3 diffuse = light.props.diffuse * diffusionAngle * diffuseMap * attenuation;
    vec3 specular = light.props.specular * specularMultiplier * specularMap * attenuation;
    return (ambient + diffuse + specular);
}

vec3 calcSpotLight(Spotlight light, vec3 normal, vec3 viewDir, vec3 diffuseMap, vec3 specularMap)
{
    // Points from the light source to the object (inwards)
    vec3 lightDir = normalize(light.pointLight.props.position - a_FragPos);
    // Diffuse shading
    float diffusionAngle = max(dot(normal, lightDir), 0.0);
    // Reflect the *negated* light vector w.r.t to the normal vector
    // We negate the light vector since we want the reflection to point
    vec3 reflectDir = reflect(-lightDir, normal);
    float specularMultiplier = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.shininess);
    // attenuation
    float attenuation = calcAttenuation(light.pointLight);
    // Calcualte spotlight
    float spotlightIntensity = calcSpotlightIntensity(light, lightDir);
    // Combine results
    vec3 ambient = light.pointLight.props.ambient * diffuseMap * attenuation;
    vec3 diffuse = light.pointLight.props.diffuse * diffusionAngle * diffuseMap * attenuation * spotlightIntensity;
    vec3 specular = light.pointLight.props.specular * specularMultiplier * specularMap * attenuation * spotlightIntensity;
    return (ambient + diffuse + specular);
}

void main()
{
    // Generate diffuse and specular maps
    vec3 diffuseMap = texture(u_Material.diffuse, a_TexCoord).rgb;
    vec3 specularMap = texture(u_Material.specular, a_TexCoord).rgb;
    // TODO: add emissions if necessary or drop
    //    // Transforms all non-zero -> 0 and 0 -> 1
    //    vec3 invertedSpecularMap = abs(ceil(specularMap) - vec3(1.0));
    //    vec3 emissionMap = invertedSpecularMap * u_EmissionLight * texture(u_Emission, a_TexCoord).rgb;

    // Properties
    vec3 norm = normalize(a_NormalVec);
    vec3 viewDir = normalize(u_ViewerPos - a_FragPos);

    // Phase 1: Directional lighting
    vec3 result = calcDirLight(u_DirectLight[0], norm, viewDir, diffuseMap, specularMap);
    // Phase 2: Point lights
    for (int i = 0; i < POINT_LIGHT_COUNT; ++i) {
        result += calcPointLight(u_PointLight[i], norm, viewDir, diffuseMap, specularMap) /*+ emissionMap*/;
    }
    //    // Phase 3: Spot light
    //    result += calcSpotLight(u_Spotlight[0], norm, viewDir, diffuseMap, specularMap);

    fragColor = vec4(result, 1.0);
}