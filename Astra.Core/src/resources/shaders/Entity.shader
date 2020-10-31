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

out float v_attenuation;

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

in vec2 v_TexCoordinates;
in vec3 v_Normal;
in vec3 v_FragPosition;
in vec3 v_viewVector;
in float v_attenuation;

out vec4 out_Color;

struct Material
{
	sampler2D diffuseMap;
	sampler2D specularMap;
	float reflectivity;
};
uniform Material material;

struct Light
{
	vec4 l_vector;
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	vec3 attenuation;
	float cutOff;
	float outerCutOff;
};
uniform Light light;

void main()
{
	vec4 textureColor = texture(material.diffuseMap, v_TexCoordinates);
	if (textureColor.a < 0.5) { discard; }

	vec3 toLight = light.l_vector.xyz - v_FragPosition;
	vec3 lightDir = light.l_vector.w < 0.5 ? normalize(-light.direction) : normalize(toLight);
	
	float distance = length(toLight);
	float atten = light.attenuation.x > 0.5 ? 1.0 / (light.attenuation.x + (light.attenuation.y * distance) + (light.attenuation.z * (distance * distance))) : 1;
	atten = max(atten, 0.2);

	vec3 color = textureColor.rgb;

	// Ambient
	vec3 ambient = light.ambient * color * atten;

	// Diffuse
	vec3 norm = normalize(v_Normal);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diff * color * atten;
	
	// Specular
	vec3 viewDir = normalize(v_viewVector);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.reflectivity);
	vec3 specular = light.specular * spec * texture(material.specularMap, v_TexCoordinates).rgb * atten;

	float theta = dot(lightDir, normalize(-light.direction));
	if (light.cutOff < 0.5 || theta > light.cutOff)
	{
		out_Color = vec4(ambient + diffuse + specular, textureColor.a);
	}
	else
	{
		float epsilon = light.cutOff - light.outerCutOff;
		float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

		diffuse *= intensity;
		specular *= intensity;
		out_Color = vec4(ambient + diffuse + specular, textureColor.a);
	}
}