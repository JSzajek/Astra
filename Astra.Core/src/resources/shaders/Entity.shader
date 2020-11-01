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

out vec2 v_TexCoordinates;
out vec3 v_Normal;
out vec3 v_FragPosition;
out vec3 v_viewVector;

void main()
{
	vec4 worldPosition = transformMatrix * vec4(position, 1);

	v_TexCoordinates = textureCoords;
	v_Normal = mat3(normalMatrix) * normal;
	v_FragPosition = worldPosition.xyz;
	v_viewVector = inverseViewVector.xyz - v_FragPosition;

	gl_Position = projectionMatrix * viewMatrix * worldPosition;
}

#shader fragment
#version 460

#define NR_POINT_LIGHTS %i

in vec2 v_TexCoordinates;
in vec3 v_Normal;
in vec3 v_FragPosition;
in vec3 v_viewVector;

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

vec3 CalcDirLight(DirectionalLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
	vec3 norm = normalize(v_Normal);
	vec3 viewDir = normalize(v_viewVector);

	vec3 result = CalcDirLight(directionalLight, norm, viewDir);
	for (int i = 0; i < NR_POINT_LIGHTS; i++)
	{
		result += CalcPointLight(pointLights[0], norm, v_FragPosition, viewDir);
	}
	result += CalcSpotLight(spotLight, norm, v_FragPosition, viewDir);

	out_Color = vec4(result, 1.0);
}

vec3 CalcDirLight(DirectionalLight light, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = normalize(-light.direction);
	
	// diffuse shading
	float diff = max(dot(normal, lightDir), 0.0);
	
	// specular shading
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.reflectivity);
	
	// combine results
	vec4 textureColor = texture(material.diffuseMap, v_TexCoordinates);
	vec3 color = textureColor.rgb;

	vec3 ambient = light.ambient * color;
	vec3 diffuse = light.diffuse * diff * color;
	vec3 specular = light.specular * spec * texture(material.specularMap, v_TexCoordinates).rgb;
	return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - fragPos);
	
	// diffuse shading
	float diff = max(dot(normal, lightDir), 0.0);
	
	// specular shading
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.reflectivity);
	
	// attenuation
	float distance = length(light.position - fragPos);
	float attenuation = 1.0 / (light.attenuation.x + light.attenuation.y * distance + light.attenuation.z * (distance * distance));
	
	// combine results
	vec4 textureColor = texture(material.diffuseMap, v_TexCoordinates);
	vec3 color = textureColor.rgb;

	vec3 ambient = light.ambient * color;
	vec3 diffuse = light.diffuse * diff * color;
	vec3 specular = light.specular * spec * texture(material.specularMap, v_TexCoordinates).rgb;
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;
	return (ambient + diffuse + specular);
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	if (light.cutOff < 0.02) { return vec3(0); }

	vec3 lightDir = normalize(light.position - fragPos);
	
	// diffuse shading
	float diff = max(dot(normal, lightDir), 0.0);
	
	// specular shading
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.reflectivity);
	
	// attenuation
	float distance = length(light.position - fragPos);
	float attenuation = 1.0 / (light.attenuation.x + light.attenuation.y * distance + light.attenuation.z * (distance * distance));
	
	// spotlight intensity
	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon = light.cutOff - light.outerCutOff;
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
	
	// combine results
	vec4 textureColor = texture(material.diffuseMap, v_TexCoordinates);
	vec3 color = textureColor.rgb;

	vec3 ambient = light.ambient * color;
	vec3 diffuse = light.diffuse * diff * color;
	vec3 specular = light.specular * spec * texture(material.specularMap, v_TexCoordinates).rgb;
	ambient *= attenuation * intensity;
	diffuse *= attenuation * intensity;
	specular *= attenuation * intensity;
	return (ambient + diffuse + specular);
}