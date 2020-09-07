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

out vec3 surfaceNormal;
out vec3 toLightVector;
out vec3 toCameraVector;

out float visibility;

const float density = 0.0035;
const float gradient = 5.0;

void main()
{
	v_TexCoordinates = textureCoords;
	vec4 worldPosition = transformMatrix * vec4(position, 1);
	vec4 positionRelativeToCam = viewMatrix * worldPosition;
	gl_Position = projectionMatrix * positionRelativeToCam;

	surfaceNormal = (transformMatrix * vec4(normal, 0)).xyz;
	toLightVector = lightPosition - worldPosition.xyz;
	toCameraVector = (inverse(viewMatrix) * vec4(0, 0, 0, 1)).xyz - worldPosition.xyz;
	
	float distance = length(positionRelativeToCam.xyz);
	visibility = exp(-pow((distance * density), gradient));
	visibility = clamp(visibility, 0, 1);
}

#shader fragment
#version 460

in vec2 v_TexCoordinates;

in vec3 surfaceNormal;
in vec3 toLightVector;
in vec3 toCameraVector;

in float visibility;

out vec4 out_Color;

uniform sampler2D backgroundTexture;
uniform sampler2D rTexture;
uniform sampler2D gTexture;
uniform sampler2D bTexture;
uniform sampler2D blendMap;

uniform vec3 lightColor;
uniform float shineDampener;
uniform float reflectivity;

uniform vec3 skyColor;

void main()
{
	vec4 blendMapColor = texture(blendMap, v_TexCoordinates);
	float backTextureAmount = 1 - (blendMapColor.r + blendMapColor.g + blendMapColor.b);
	vec2 tiledCoords = v_TexCoordinates * 40;
	vec4 backgroundTextureColor = texture(backgroundTexture, tiledCoords) * backTextureAmount;
	vec4 rTextureColor = texture(rTexture, tiledCoords) * blendMapColor.r;
	vec4 gTextureColor = texture(gTexture, tiledCoords) * blendMapColor.g;
	vec4 bTextureColor = texture(bTexture, tiledCoords) * blendMapColor.b;

	vec4 totalColor = backgroundTextureColor + rTextureColor + gTextureColor + bTextureColor;

	vec3 unitNormal = normalize(surfaceNormal);
	vec3 unitLightVector = normalize(toLightVector);

	float nDot1 = dot(unitNormal, unitLightVector);
	float brightness = max(nDot1, 0.2);
	vec3 diffuse = brightness * lightColor;

	vec3 unitVectorToCamera = normalize(toCameraVector);
	vec3 lightDirection = -unitLightVector;
	vec3 reflectedLightDirection = reflect(lightDirection, unitNormal);

	float specularFactor = dot(reflectedLightDirection, unitVectorToCamera);
	specularFactor = max(specularFactor, 0);
	float dampenedFactor = pow(specularFactor, shineDampener);
	vec3 finalSpecular = dampenedFactor * reflectivity * lightColor;

	out_Color = vec4(diffuse, 1) * totalColor + vec4(finalSpecular, 1);
	out_Color = mix(vec4(skyColor, 1), out_Color, visibility);
}