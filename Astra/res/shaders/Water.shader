#shader vertex
#version 330

layout(location = 0) in vec2 position;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 transformMatrix;
uniform mat4 normalMatrix;
uniform mat4 toShadowMapSpace;

uniform vec4 inverseViewVector;
uniform float shadowDistance;
uniform float transitionDistance;

out vec2 v_TexCoordinates;
out vec3 v_FragPosition;
out vec3 v_viewVector;
out vec4 v_ClipSpace;
out float v_Visibility;
out vec4 v_ShadowCoords;

const float tiling = 3.0;
const float density = 0.0035;
const float gradient = 5.0;

void main()
{
	vec4 worldPosition = transformMatrix * vec4(position.x, 0.0, position.y, 1.0);
	v_ShadowCoords = toShadowMapSpace * worldPosition;
	vec4 positionRelativeToCam = viewMatrix * worldPosition;
	v_ClipSpace = projectionMatrix * positionRelativeToCam;
	
	v_TexCoordinates = vec2(position.x / 2.0 + 0.5, position.y / 2.0 + 0.5) * tiling;
	v_FragPosition = worldPosition.xyz;
	v_viewVector = inverseViewVector.xyz - v_FragPosition;

	float distance = length(positionRelativeToCam.xyz);
	v_Visibility = exp(-pow((distance * density), gradient));
	v_Visibility = clamp(v_Visibility, 0, 1);

	distance = distance - (shadowDistance - transitionDistance);
	distance = distance / transitionDistance;
	v_ShadowCoords.w = clamp(1.0 - distance, 0.0, 1.0);
	
	gl_Position = v_ClipSpace;
}

#shader fragment
#version 330

#define NR_POINT_LIGHTS %i
#define BLINN 1

in vec2 v_TexCoordinates;
in vec3 v_FragPosition;
in vec3 v_viewVector;
in vec4 v_ClipSpace;
in float v_Visibility;
in vec4 v_ShadowCoords;

out vec4 out_Color;

struct Material
{
	sampler2D diffuseMap;
	sampler2D reflectionTexture;
	sampler2D refractionTexture;
	sampler2D dudvMap;
	sampler2D normalMap;
	sampler2D specularMap;
	sampler2D shadowMap;
	sampler2D depthMap;
	float reflectivity;
};
uniform Material material;

struct DirectionalLight
{
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
uniform DirectionalLight directionalLight;

struct PointLight
{
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	vec3 attenuation;
};
uniform PointLight pointLights[NR_POINT_LIGHTS];

struct SpotLight
{
	vec3 position;
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float cutOff;
	float outerCutOff;
	vec3 attenuation;
};
uniform SpotLight spotLight;

const float kPi = 3.14159265;

uniform float mapSize;
uniform int pcfCount;

uniform float nearPlane;
uniform float farPlane;

uniform float moveFactor;
uniform float waveStrength;

vec3 CalcDirLight(DirectionalLight light, vec3 normal, vec3 specColor, vec3 viewDir, float waterDepth, float lightFactor);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 specColor, vec3 viewDir, float waterDepth, float lightFactor);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 specColor, vec3 viewDir, float waterDepth, float lightFactor);

void main()
{
	vec3 viewDir = normalize(v_viewVector);

	vec2 normDeviceSpace = (v_ClipSpace.xy / v_ClipSpace.w) / 2.0 + 0.5;
	vec2 reflectTexCoords = vec2(normDeviceSpace.x, -normDeviceSpace.y);

	// water depth = floor distance - water distance
	float waterDepth = (2.0 * nearPlane * farPlane / (farPlane + nearPlane - (2.0 * texture(material.depthMap, normDeviceSpace.xy).r - 1.0) * (farPlane - nearPlane)))
						- (2.0 * nearPlane * farPlane / (farPlane + nearPlane - (2.0 * gl_FragCoord.z - 1.0) * (farPlane - nearPlane)));

	vec2 distortedTexCoords = texture(material.dudvMap, vec2(v_TexCoordinates.x + moveFactor, v_TexCoordinates.y)).rg * 0.1;
	distortedTexCoords = v_TexCoordinates + vec2(distortedTexCoords.x, distortedTexCoords.y + moveFactor);
	vec2 totalDistortion = (texture(material.dudvMap, distortedTexCoords).rg * 2.0 - 1.0) * waveStrength * clamp(waterDepth / 10.0, 0, 1);

	normDeviceSpace += totalDistortion;
	normDeviceSpace = clamp(normDeviceSpace, 0.001, 0.999);

	reflectTexCoords += totalDistortion;
	reflectTexCoords.x = clamp(reflectTexCoords.x, 0.001, 0.999);
	reflectTexCoords.y = clamp(reflectTexCoords.y, -0.999, -0.001);

	vec4 refractColor = texture(material.refractionTexture, normDeviceSpace);
	vec4 reflectColor = texture(material.reflectionTexture, reflectTexCoords);

	vec4 normalMapColor = texture(material.normalMap, distortedTexCoords);
	vec3 normal = normalize(vec3(normalMapColor.r * 2.0 - 1.0, normalMapColor.b * 3.0, normalMapColor.g * 2.0 - 1.0));

	float refractiveFactor = dot(viewDir, normal);
	refractiveFactor = pow(refractiveFactor, 2); // High - Higher reflectivity, Low - Lower reflectivity

	vec3 color = reflectColor.rgb;
	vec3 specColor = texture(material.specularMap, v_TexCoordinates).rgb;

	float texelSize = 1.0 / mapSize;
	float total = 0.0;
	for (int x = -pcfCount; x <= pcfCount; x++)
	{
		for (int y = -pcfCount; y <= pcfCount; y++)
		{
			if (v_ShadowCoords.z > texture(material.shadowMap, v_ShadowCoords.xy + vec2(x, y) * texelSize).r + 0.002)
			{
				total += 1.0;
			}
		}
	}

	total /= (pcfCount * 2.0 + 1.0) * (pcfCount * 2.0 + 1.0);;
	float lightFactor = 1.0 - (total * v_ShadowCoords.w);
	
	vec3 totalReflective = CalcDirLight(directionalLight, normal, specColor, viewDir, waterDepth, lightFactor);
	for (int i = 0; i < NR_POINT_LIGHTS; i++)
	{
		totalReflective += CalcPointLight(pointLights[i], normal, specColor, viewDir, waterDepth, lightFactor);
	}
	totalReflective += CalcSpotLight(spotLight, normal, specColor, viewDir, waterDepth, lightFactor);
	totalReflective *= color;

	out_Color = mix(vec4(totalReflective, reflectColor.a), refractColor, refractiveFactor);
	out_Color = mix(out_Color, texture(material.diffuseMap, vec2(0)), 0.25);
	out_Color.a = clamp(waterDepth / 5.0, 0, 1);
	out_Color.a = min(out_Color.a, v_Visibility);
}

vec3 CalcDirLight(DirectionalLight light, vec3 normal, vec3 specColor, vec3 viewDir, float waterDepth, float lightFactor)
{
	vec3 lightDir = normalize(-light.direction);

	// diffuse shading
	float diff = max(dot(normal, lightDir), 0.0) * lightFactor;

	// specular shading
#if BLINN
	float kEnergyConservation = (8.0 + material.reflectivity) / (8.0 * kPi);
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = kEnergyConservation * pow(max(dot(normal, halfwayDir), 0), material.reflectivity);
#else
	float kEnergyConservation = (2.0 + material.reflectivity) / (2.0 * kPi);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = kEnergyConservation * pow(max(dot(viewDir, reflectDir), 0), material.reflectivity);
#endif

	// combine results
	vec3 ambient = light.ambient;
	vec3 diffuse = light.diffuse * diff;
	vec3 specular = light.specular * spec * specColor * clamp(waterDepth / 5.0, 0, 1);
	return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 specColor, vec3 viewDir, float waterDepth, float lightFactor)
{
	vec3 lightDir = normalize(light.position - v_FragPosition);

	// diffuse shading
	float diff = max(dot(normal, lightDir), 0.0) * lightFactor;

	// specular shading
#if BLINN
	float kEnergyConservation = (8.0 + material.reflectivity) / (8.0 * kPi);
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = kEnergyConservation * pow(max(dot(normal, halfwayDir), 0), material.reflectivity);
#else
	float kEnergyConservation = (2.0 + material.reflectivity) / (2.0 * kPi);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = kEnergyConservation * pow(max(dot(viewDir, reflectDir), 0), material.reflectivity);
#endif

	// attenuation
	float distance = length(light.position - v_FragPosition);
	float attenuation = 1.0 / (light.attenuation.x + light.attenuation.y * distance + light.attenuation.z * (distance * distance));

	// combine results
	vec3 ambient = light.ambient;
	vec3 diffuse = light.diffuse * diff;
	vec3 specular = light.specular * spec * specColor * clamp(waterDepth / 5.0, 0, 1);
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;
	return (ambient + diffuse + specular);
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 specColor, vec3 viewDir, float waterDepth, float lightFactor)
{
	if (light.cutOff < 0.02) { return vec3(0); }

	vec3 lightDir = normalize(light.position - v_FragPosition);

	// diffuse shading
	float diff = max(dot(normal, lightDir), 0.0) * lightFactor;

	// specular shading
#if BLINN
	float kEnergyConservation = (8.0 + material.reflectivity) / (8.0 * kPi);
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = kEnergyConservation * pow(max(dot(normal, halfwayDir), 0), material.reflectivity);
#else
	float kEnergyConservation = (2.0 + material.reflectivity) / (2.0 * kPi);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = kEnergyConservation * pow(max(dot(viewDir, reflectDir), 0), material.reflectivity);
#endif

	// attenuation
	float distance = length(light.position - v_FragPosition);
	float attenuation = 1.0 / (light.attenuation.x + light.attenuation.y * distance + light.attenuation.z * (distance * distance));

	// spotlight intensity
	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon = light.cutOff - light.outerCutOff;
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

	// combine results
	vec3 ambient = light.ambient;
	vec3 diffuse = light.diffuse * diff;
	vec3 specular = light.specular * spec * specColor * clamp(waterDepth / 5.0, 0, 1);
	ambient *= attenuation * intensity;
	diffuse *= attenuation * intensity;
	specular *= attenuation * intensity;
	return (ambient + diffuse + specular);
}