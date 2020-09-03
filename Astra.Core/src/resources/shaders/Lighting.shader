#shader vertex
#version 460

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 textureCoords;
layout(location = 2) in vec3 normal;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 transformMatrix;

uniform vec3 lightPosition;

out vec2 v_TexCoordinates;
out float v_discard;

out vec3 unitNormal;
out vec3 toLightVector;
out vec3 unitCameraVector;

void main()
{
	v_TexCoordinates = textureCoords;
	vec4 worldPosition = transformMatrix * vec4(position, 1);
	gl_Position = projectionMatrix * viewMatrix * worldPosition;

	unitNormal = normalize((transformMatrix * vec4(normal, 0)).xyz);
	toLightVector = lightPosition - worldPosition.xyz;
	unitCameraVector = normalize((inverse(viewMatrix) * vec4(0, 0, 0, 1)).xyz - worldPosition.xyz);
}

#shader fragment
#version 460

in vec2 v_TexCoordinates;
in float v_discard;

in vec3 unitNormal;
in vec3 toLightVector;
in vec3 unitCameraVector;

out vec4 out_Color;

uniform sampler2D u_Texture;

uniform vec3 lightColor;
uniform float shineDampener;
uniform float reflectivity;

void main()
{
	vec3 unitLightVector = normalize(toLightVector);

	float brightness = dot(unitNormal, unitLightVector);
	brightness = max(brightness, 0.2);
	vec3 diffuse = brightness * lightColor;

	vec3 lightDirection = -unitLightVector;
	vec3 reflectedLightDirection = reflect(lightDirection, unitNormal);

	float specularFactor = dot(reflectedLightDirection, unitCameraVector);
	specularFactor = max(specularFactor, 0);
	float dampenedFactor = pow(specularFactor, shineDampener);
	vec3 finalSpecular = dampenedFactor * reflectivity * lightColor;

	vec4 textureColor = texture2D(u_Texture, v_TexCoordinates);
	if (textureColor.a < 0.5) { discard; }

	out_Color = vec4(diffuse, 1) * textureColor + vec4(finalSpecular, 1);
}