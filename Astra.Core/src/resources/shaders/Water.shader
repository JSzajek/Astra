#shader vertex
#version 460

layout(location = 0) in vec2 position;

out vec4 clipSpace;
out vec2 v_TexCoordinates;
out vec3 toCameraVector;
out vec3 fromLightVector;

uniform mat4 projectionMatrix = mat4(1.0);
uniform mat4 viewMatrix = mat4(1.0);
uniform mat4 transformMatrix = mat4(1.0);

uniform vec3 lightPosition;

uniform vec3 cameraPosition;

const float tiling = 3.0;

void main()
{
	vec4 worldPosition = transformMatrix * vec4(position.x, 0, position.y, 1.0);
	clipSpace = projectionMatrix * viewMatrix * worldPosition;
	gl_Position = clipSpace;
	v_TexCoordinates = vec2(position.x / 2.0 + 0.5, position.y / 2.0 + 0.5) * tiling;

	toCameraVector = cameraPosition - worldPosition.xyz;
	fromLightVector = worldPosition.xyz - lightPosition;
}

#shader fragment
#version 460

out vec4 out_Color;

in vec4 clipSpace;
in vec2 v_TexCoordinates;
in vec3 toCameraVector;
in vec3 fromLightVector;

uniform float nearPlane;
uniform float farPlane;

uniform sampler2D reflectionTexture;
uniform sampler2D refractionTexture;
uniform sampler2D dudvMap;
uniform sampler2D normalMap;
uniform sampler2D depthMap;

uniform float moveFactor;
uniform float waveStrength;
uniform float shineDampener;
uniform float reflectivity;
uniform vec4 baseWaterColor;

uniform vec3 lightColor;

void main()
{
	vec2 normDeviceSpace = (clipSpace.xy / clipSpace.w) / 2.0 + 0.5;
	vec2 reflectTexCoords = vec2(normDeviceSpace.x, -normDeviceSpace.y);

	// water depth = floor distance - water distance
	float waterDepth = (2.0 * nearPlane * farPlane / (farPlane + nearPlane - (2.0 * texture(depthMap, normDeviceSpace).r - 1.0) * (farPlane - nearPlane))) 
						- (2.0 * nearPlane * farPlane / (farPlane + nearPlane - (2.0 * gl_FragCoord.z - 1.0) * (farPlane - nearPlane)));

	vec2 distortedTexCoords = texture(dudvMap, vec2(v_TexCoordinates.x + moveFactor, v_TexCoordinates.y)).rg * 0.1;
	distortedTexCoords = v_TexCoordinates + vec2(distortedTexCoords.x, distortedTexCoords.y + moveFactor);
	vec2 totalDistortion = (texture(dudvMap, distortedTexCoords).rg * 2.0 - 1.0) * waveStrength * clamp(waterDepth / 10, 0, 1);
	
	normDeviceSpace += totalDistortion;
	normDeviceSpace = clamp(normDeviceSpace, 0.001, 0.999);
	
	reflectTexCoords += totalDistortion;
	reflectTexCoords.x = clamp(reflectTexCoords.x, 0.001, 0.999);
	reflectTexCoords.y = clamp(reflectTexCoords.y, -0.999, -0.001);
	
	vec4 refractColor = texture(refractionTexture, normDeviceSpace);
	vec4 reflectColor = texture(reflectionTexture, reflectTexCoords);
	
	vec4 normalMapColor = texture(normalMap, distortedTexCoords);
	vec3 normal = normalize(vec3(normalMapColor.r * 2.0 - 1.0, normalMapColor.b * 3.0, normalMapColor.g * 2.0 - 1.0));
	
	vec3 viewVector = normalize(toCameraVector);
	float refractiveFactor = dot(viewVector, normal);
	refractiveFactor = pow(refractiveFactor, 10); // High - Higher reflectivity, Low - Lower reflectivity

	vec3 reflectedLight = reflect(normalize(fromLightVector), normal);
	float specular = pow(max(dot(reflectedLight, viewVector), 0.0), shineDampener);
	vec3 specularHighlights = lightColor * specular * reflectivity * clamp(waterDepth / 5, 0, 1);

	out_Color = mix(reflectColor, refractColor, refractiveFactor);
	out_Color = mix(out_Color, baseWaterColor, 0.2) + vec4(specularHighlights, 0);
	out_Color.a = clamp(waterDepth / 5, 0, 1);
}