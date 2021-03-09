#shader vertex
#version 330

#define MAX_BONES			75
#define MAX_BONE_INFLUENCE	3

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 textureCoords;
layout(location = 2) in vec3 normal;
layout(location = 3) in ivec3 boneIds;
layout(location = 4) in vec3 weights;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 transformMatrix;
uniform mat4 normalMatrix;
uniform mat4 toShadowMapSpace;

uniform int animated = 0;
uniform mat4 boneTransformation[MAX_BONES];

uniform vec4 inverseViewVector;
uniform int useFakeLighting;
uniform float numberOfRows;
uniform vec2 offset;
uniform vec4 clipPlane;
uniform float shadowDistance;
uniform float transitionDistance;

out vec2 v_TexCoordinates;
out vec3 v_Normal;
out vec3 v_FragPosition;
out vec3 v_viewVector;
out float v_Visibility;
out vec4 v_ShadowCoords;

const float density = 0.0035;
const float gradient = 5.0;

void CalcBoneInfluence(inout vec4 totalPosition, inout vec4 totalNormal);

void main()
{
	vec4 aPosition = vec4(position, 1.0f);
	vec3 aNormal = normal;
	if (animated > 0)
	{
		vec4 totalPosition = vec4(0.0f);
		vec4 totalNormal = vec4(0.0f);
		CalcBoneInfluence(totalPosition, totalNormal);
		
		aPosition = totalPosition;
		aNormal = totalNormal.xyz;
	}
	vec4 worldPosition = transformMatrix * aPosition;

	v_ShadowCoords = toShadowMapSpace * worldPosition;
	vec4 positionRelativeToCam = viewMatrix * worldPosition;

	gl_ClipDistance[0] = dot(worldPosition, clipPlane);

	v_TexCoordinates = (textureCoords / numberOfRows) + offset;

	v_Normal = useFakeLighting > 0.5 ? vec3(0, 1, 0) : mat3(normalMatrix) * aNormal;
	v_FragPosition = worldPosition.xyz;
	v_viewVector = inverseViewVector.xyz - v_FragPosition;

	float distance = length(positionRelativeToCam.xyz);
	v_Visibility = exp(-pow((distance * density), gradient));
	v_Visibility = clamp(v_Visibility, 0, 1);

	distance = distance - (shadowDistance - transitionDistance);
	distance = distance / transitionDistance;
	v_ShadowCoords.w = clamp(1.0 - distance, 0.0, 1.0);

	gl_Position = projectionMatrix * positionRelativeToCam;
}

void CalcBoneInfluence(inout vec4 totalPosition, inout vec4 totalNormal)
{
	vec4 partialPos = vec4(position, 1.0f);
	vec4 partialNorm = vec4(normal, 1.0f);

	for (int i = 0; i < MAX_BONE_INFLUENCE; ++i)
	{
		if (boneIds[i] >= 0 && boneIds[i] < MAX_BONES)
		{
			totalPosition += boneTransformation[boneIds[i]] * partialPos * weights[i];
			totalNormal += boneTransformation[boneIds[i]] * partialNorm * weights[i];
		}
	}
}

#shader fragment
#version 330

#define NR_POINT_LIGHTS %i
#define BLINN 1

in vec2 v_TexCoordinates;
in vec3 v_Normal;
in vec3 v_FragPosition;
in vec3 v_viewVector;
in float v_Visibility;
in vec4 v_ShadowCoords;

out vec4 out_Color;

struct Material
{
	sampler2D diffuseMap;
	sampler2D specularMap;
	sampler2D emissionMap;
	sampler2D shadowMap;
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

uniform int glowing;

const float kPi = 3.14159265;

uniform vec3 fogColor;
uniform float mapSize;
uniform int pcfCount;

vec3 CalcDirLight(DirectionalLight light, vec3 normal, vec3 specColor, vec3 viewDir, float lightFactor);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 specColor, vec3 viewDir, float lightFactor);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 specColor, vec3 viewDir, float lightFactor);

void main()
{
	vec3 norm = normalize(v_Normal);
	vec3 viewDir = normalize(v_viewVector);

	vec4 textureColor = texture(material.diffuseMap, v_TexCoordinates);
	if (textureColor.a < 0.5) { discard; }
	vec3 color = textureColor.rgb;
	vec3 specColor = texture(material.specularMap, v_TexCoordinates).rgb;
	vec3 glowingColor = !(glowing > 0) ? vec3(0) : texture(material.emissionMap, v_TexCoordinates).rgb;
	
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

	vec3 result = CalcDirLight(directionalLight, norm, specColor, viewDir, lightFactor);
	for (int i = 0; i < NR_POINT_LIGHTS; i++)
	{
		result += CalcPointLight(pointLights[i], norm, specColor, viewDir, lightFactor);
	}
	result += CalcSpotLight(spotLight, norm, specColor, viewDir, lightFactor);
	result *= color;

	out_Color = vec4(result + glowingColor * 2, textureColor.a);
	out_Color = mix(vec4(fogColor, 1), out_Color, v_Visibility);
}

vec3 CalcDirLight(DirectionalLight light, vec3 normal, vec3 specColor, vec3 viewDir, float lightFactor)
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
	vec3 specular = light.specular * spec * specColor;
	return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 specColor, vec3 viewDir, float lightFactor)
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
	vec3 specular = light.specular * spec * specColor;
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;
	return (ambient + diffuse + specular);
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 specColor, vec3 viewDir, float lightFactor)
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
	vec3 specular = light.specular * spec * specColor;
	ambient *= attenuation * intensity;
	diffuse *= attenuation * intensity;
	specular *= attenuation * intensity;
	return (ambient + diffuse + specular);
}