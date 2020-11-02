#shader vertex
#version 460

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 textureCoords;
layout(location = 2) in vec3 normal;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 transformMatrix;
uniform mat4 normalMatrix;

uniform vec4 inverseViewVector;
uniform float useFakeLighting;
uniform float numberOfRows;
uniform vec2 offset;
uniform vec4 clipPlane;

out vec2 v_TexCoordinates;
out vec3 v_Normal;
out vec3 v_FragPosition;
out vec3 v_viewVector;
out float visibility;

const float density = 0.0035;
const float gradient = 5.0;

void main()
{
	vec4 worldPosition = transformMatrix * vec4(position, 1);
	vec4 positionRelativeToCam = viewMatrix * worldPosition;

	gl_ClipDistance[0] = dot(worldPosition, clipPlane);

	v_TexCoordinates = (textureCoords / numberOfRows) + offset;

	v_Normal = useFakeLighting > 0.5 ? vec3(0, 1, 0) : mat3(normalMatrix) * normal;
	v_FragPosition = worldPosition.xyz;
	v_viewVector = inverseViewVector.xyz - v_FragPosition;

	float distance = length(positionRelativeToCam.xyz);
	visibility = exp(-pow((distance * density), gradient));
	visibility = clamp(visibility, 0, 1);

	gl_Position = projectionMatrix * positionRelativeToCam;
}

#shader fragment
#version 460

#define NR_POINT_LIGHTS %i
#define BLINN 1

in vec2 v_TexCoordinates;
in vec3 v_Normal;
in vec3 v_FragPosition;
in vec3 v_viewVector;
in float visibility;

out vec4 out_Color;

struct Material
{
	sampler2D diffuseMap;
	sampler2D specularMap;
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

uniform vec3 fogColor;

vec3 CalcDirLight(DirectionalLight light, vec3 normal, vec3 color, vec3 specColor, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 color, vec3 specColor, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 color, vec3 specColor, vec3 viewDir);

void main()
{
	vec3 norm = normalize(v_Normal);
	vec3 viewDir = normalize(v_viewVector);

	vec4 textureColor = texture(material.diffuseMap, v_TexCoordinates);
	vec3 color = textureColor.rgb;

	vec3 specColor = texture(material.specularMap, v_TexCoordinates).rgb;

	vec3 result = CalcDirLight(directionalLight, norm, color, specColor, viewDir);
	for (int i = 0; i < NR_POINT_LIGHTS; i++)
	{
		result += CalcPointLight(pointLights[0], norm, color, specColor, viewDir);
	}
	result += CalcSpotLight(spotLight, norm, color, specColor, viewDir);

	out_Color = vec4(result, 1.0);
	out_Color = mix(vec4(fogColor, 1), out_Color, visibility);
}

vec3 CalcDirLight(DirectionalLight light, vec3 normal, vec3 color, vec3 specColor, vec3 viewDir)
{
	vec3 lightDir = normalize(-light.direction);
	
	// diffuse shading
	float diff = max(dot(normal, lightDir), 0.0);
	
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
	vec3 ambient = light.ambient * color;
	vec3 diffuse = light.diffuse * diff * color;
	vec3 specular = light.specular * spec * specColor;
	return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 color, vec3 specColor, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - v_FragPosition);
	
	// diffuse shading
	float diff = max(dot(normal, lightDir), 0.0);
	
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
	vec3 ambient = light.ambient * color;
	vec3 diffuse = light.diffuse * diff * color;
	vec3 specular = light.specular * spec * specColor;
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;
	return (ambient + diffuse + specular);
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 color, vec3 specColor, vec3 viewDir)
{
	if (light.cutOff < 0.02) { return vec3(0); }

	vec3 lightDir = normalize(light.position - v_FragPosition);
	
	// diffuse shading
	float diff = max(dot(normal, lightDir), 0.0);
	
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
	vec3 ambient = light.ambient * color;
	vec3 diffuse = light.diffuse * diff * color;
	vec3 specular = light.specular * spec * specColor;
	ambient *= attenuation * intensity;
	diffuse *= attenuation * intensity;
	specular *= attenuation * intensity;
	return (ambient + diffuse + specular);
}