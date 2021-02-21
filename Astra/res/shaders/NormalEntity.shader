#shader vertex
#version 330

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 textureCoords;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec3 tangent;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 transformMatrix;
uniform mat4 normalMatrix;
uniform mat4 toShadowMapSpace;

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
out vec3 v_ViewVector;
out float v_Visibility;
out vec4 v_ShadowCoords;
out mat3 v_ToTangentSpace;

const float density = 0.0035;
const float gradient = 5.0;

void main()
{
	vec4 worldPosition = transformMatrix * vec4(position, 1);
	v_ShadowCoords = toShadowMapSpace * worldPosition;
	vec4 positionRelativeToCam = viewMatrix * worldPosition;
	
	gl_ClipDistance[0] = dot(worldPosition, clipPlane);

	v_TexCoordinates = (textureCoords / numberOfRows) + offset;

	v_FragPosition = worldPosition.xyz;
	v_Normal = useFakeLighting > 0.5 ? vec3(0, 1, 0) : mat3(normalMatrix) * normal;
	v_ViewVector = inverseViewVector.xyz - v_FragPosition;

	float distance = length(positionRelativeToCam.xyz);
	v_Visibility = exp(-pow((distance * density), gradient));
	v_Visibility = clamp(v_Visibility, 0, 1);

	distance = distance - (shadowDistance - transitionDistance);
	distance = distance / transitionDistance;
	v_ShadowCoords.w = clamp(1.0 - distance, 0.0, 1.0);

	gl_Position = projectionMatrix * positionRelativeToCam;

	mat4 modelViewMatrix = viewMatrix * transformMatrix;

	//Gram-Schmidt process to re-orthogonalize TBN
	vec3 T = normalize((normalMatrix * vec4(tangent, 0.0)).xyz);
	vec3 N = normalize((normalMatrix * vec4(normal, 0.0)).xyz);
	vec3 B = cross(N, T);
	T = normalize(T - dot(T, N) * N);
	v_ToTangentSpace = transpose(mat3(T, B, N));

	v_ViewVector = v_ToTangentSpace * v_ViewVector;
	v_FragPosition = v_ToTangentSpace * v_FragPosition;
	v_Normal = v_ToTangentSpace * v_Normal;
}

#shader fragment
#version 330

#define NR_POINT_LIGHTS %i
#define BLINN 1

in vec2 v_TexCoordinates;
in vec3 v_FragPosition;
in vec3 v_Normal;
in vec3 v_ViewVector; 
in float v_Visibility;
in vec4 v_ShadowCoords;
in mat3 v_ToTangentSpace;

out vec4 out_Color;

struct Material
{
	sampler2D diffuseMap;
	sampler2D normalMap; 
	sampler2D specularMap;
	sampler2D parallaxMap;
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

const float kPi = 3.14159265;

// flag[0] -> 0/1 normal mapped
// flag[1] -> 0/1 parallax mapped
uniform int flags[2];
uniform int glowing;

uniform vec3 fogColor;

// Shadow Parameters
uniform float mapSize;
uniform int pcfCount;

// Parallax Parameters
uniform float heightScale;
const float minLayers = 8;
const float maxLayers = 32;

vec3 CalcDirLight(DirectionalLight light, vec3 normal, vec3 specColor, vec3 viewDir, float lightFactor);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 specColor, vec3 viewDir, float lightFactor);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 specColor, vec3 viewDir, float lightFactor);

vec2 ParallaxMapping(vec2 texCoords, vec3 viewDir);

void main()
{
	vec3 norm = flags[0] < 0.5 ? normalize(v_Normal) : normalize((2.0 * texture(material.normalMap, v_TexCoordinates) - 1.0).rgb);
	vec3 viewDir = normalize(v_ViewVector);

	vec2 texCoords = flags[1] < 0.5 ? v_TexCoordinates : ParallaxMapping(v_TexCoordinates, viewDir);
	if (texCoords.x > 1.0 || texCoords.y > 1.0 || texCoords.x < 0.0 || texCoords.y < 0.0) { discard; }

	vec4 textureColor = texture(material.diffuseMap, texCoords);
	if (textureColor.a < 0.5) { discard; }
	
	out_Color = textureColor;

	vec3 color = textureColor.rgb;
	vec3 specColor = texture(material.specularMap, texCoords).rgb;
	vec3 glowingColor = !(glowing > 0) ? vec3(0) : texture(material.emissionMap, texCoords).rgb;

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

	out_Color = vec4(result + (glowingColor * 2), textureColor.a);
	out_Color = mix(vec4(fogColor, 1), out_Color, v_Visibility);
}

vec2 ParallaxMapping(vec2 texCoords, vec3 viewDir)
{
	float numLayers = mix(maxLayers, minLayers, abs(dot(vec3(0.0, 0.0, 1.0), viewDir)));
	float layerDepth = 1.0 / numLayers;
	float currentLayerDepth = 0.0;
	vec2 deltaTexCoords = (viewDir.xy / viewDir.z * heightScale) / numLayers;

	float currentDepthMapValue = texture(material.parallaxMap, texCoords).r;
	while (currentLayerDepth < currentDepthMapValue)
	{
		// shift texture coordinates along direction of P
		texCoords -= deltaTexCoords;
		// get depthmap value at current texture coordinates
		currentDepthMapValue = texture(material.parallaxMap, texCoords).r;
		// get depth of next layer
		currentLayerDepth += layerDepth;
	}
	// get texture coordinates before collision (reverse operations)
	vec2 prevTexCoords = texCoords + deltaTexCoords;

	// get depth after and before collision for linear interpolation
	float afterDepth = currentDepthMapValue - currentLayerDepth;

	// interpolation of texture coordinates
	float weight = afterDepth / (afterDepth - (texture(material.parallaxMap, prevTexCoords).r - currentLayerDepth + layerDepth));

	return prevTexCoords * weight + texCoords * (1.0 - weight);
}

vec3 CalcDirLight(DirectionalLight light, vec3 normal, vec3 specColor, vec3 viewDir, float lightFactor)
{
	vec3 lightDir = normalize(v_ToTangentSpace * -light.direction);

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
	vec3 lightPos = (v_ToTangentSpace * light.position);
	vec3 lightDir = normalize(lightPos - v_FragPosition);

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
	float distance = length(lightPos - v_FragPosition);
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

	vec3 lightDir = normalize((v_ToTangentSpace * light.position) - v_FragPosition);

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