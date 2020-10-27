#shader vertex
#version 460

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 textureCoords;
layout(location = 2) in vec3 normal;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 transformMatrix;
uniform vec3 lightPosition[4];
uniform vec4 clipPlane;

uniform mat4 toShadowMapSpace;

out vec2 v_TexCoordinates;

out vec3 surfaceNormal;
out vec3 toLightVector[4];
out vec3 toCameraVector;

out float visibility;

out vec4 shadowCoords;

const float density = 0.0035;
const float gradient = 5.0;

const float shadowDistance = 150.0;
const float transitionDistance = 10.0;

void main()
{
	vec4 worldPosition = transformMatrix * vec4(position, 1);
	shadowCoords = toShadowMapSpace * worldPosition;

	gl_ClipDistance[0] = dot(worldPosition, clipPlane);

	vec4 positionRelativeToCam = viewMatrix * worldPosition;
	gl_Position = projectionMatrix * positionRelativeToCam;
	v_TexCoordinates = textureCoords;

	surfaceNormal = (transformMatrix * vec4(normal, 0)).xyz;
	for (int i = 0; i < 4; i++)
	{
		toLightVector[i] = lightPosition[i] - worldPosition.xyz;
	}
	toCameraVector = (inverse(viewMatrix) * vec4(0, 0, 0, 1)).xyz - worldPosition.xyz;
	
	float distance = length(positionRelativeToCam.xyz);
	visibility = exp(-pow((distance * density), gradient));
	visibility = clamp(visibility, 0, 1);

	distance = distance - (shadowDistance - transitionDistance);
	distance = distance / transitionDistance;
	shadowCoords.w = clamp(1.0 - distance, 0.0, 1.0);
}

#shader fragment
#version 460

in vec2 v_TexCoordinates;

in vec3 surfaceNormal;
in vec3 toLightVector[4];
in vec3 toCameraVector;

in float visibility;

in vec4 shadowCoords;

out vec4 out_Color;

uniform sampler2D backgroundTexture;
uniform sampler2D rTexture;
uniform sampler2D gTexture;
uniform sampler2D bTexture;
uniform sampler2D blendMap;
uniform sampler2D shadowMap;

uniform vec3 lightColor[4];
uniform vec3 attenuation[4];

uniform float shineDampener;
uniform float reflectivity;

uniform vec3 fogColor;

void main()
{
	float objectNearestLight = texture(shadowMap, shadowCoords.xy).r;
	float lightFactor = 1.0;
	if (shadowCoords.z > objectNearestLight)
	{
		lightFactor = 1.0 - (shadowCoords.w * 0.4);
	}

	vec4 blendMapColor = texture(blendMap, v_TexCoordinates);
	float backTextureAmount = 1 - (blendMapColor.r + blendMapColor.g + blendMapColor.b);
	vec2 tiledCoords = v_TexCoordinates * 40;
	vec4 backgroundTextureColor = texture(backgroundTexture, tiledCoords) * backTextureAmount;
	vec4 rTextureColor = texture(rTexture, tiledCoords) * blendMapColor.r;
	vec4 gTextureColor = texture(gTexture, tiledCoords) * blendMapColor.g;
	vec4 bTextureColor = texture(bTexture, tiledCoords) * blendMapColor.b;

	vec4 totalColor = backgroundTextureColor + rTextureColor + gTextureColor + bTextureColor;

	vec3 unitNormal = normalize(surfaceNormal);
	vec3 unitVectorToCamera = normalize(toCameraVector);

	vec3 totalDiffuse = vec3(0);
	vec3 totalSpecular = vec3(0);

	for (int i = 0; i < 4; i++)
	{
		float distance = length(toLightVector[i]);
		float attenuationFactor = attenuation[i].x + (attenuation[i].y * distance) + (attenuation[i].z * distance * distance);

		vec3 unitLightVector = normalize(toLightVector[i]);

		float nDot1 = dot(unitNormal, unitLightVector);
		float brightness = max(nDot1, 0.0);

		vec3 lightDirection = -unitLightVector;
		vec3 reflectedLightDirection = reflect(lightDirection, unitNormal);

		float specularFactor = dot(reflectedLightDirection, unitVectorToCamera);
		specularFactor = max(specularFactor, 0);
		float dampenedFactor = pow(specularFactor, shineDampener);

		totalDiffuse += (brightness * lightColor[i]) / attenuationFactor;
		totalSpecular += (dampenedFactor * reflectivity * lightColor[i]) / attenuationFactor;
	}
	totalDiffuse = max(totalDiffuse, 0.2) * lightFactor;

	out_Color = vec4(totalDiffuse, 1) * totalColor + vec4(totalSpecular, 1);
	out_Color = mix(vec4(fogColor, 1), out_Color, visibility);
}