#shader vertex
#version 460

layout(location = 0) in vec2 position;

out vec4 clipSpace;
out vec2 v_TexCoordinates;

uniform mat4 projectionMatrix = mat4(1.0);
uniform mat4 viewMatrix = mat4(1.0);
uniform mat4 transformMatrix = mat4(1.0);

const float tiling = 6.0;

void main()
{
	clipSpace = projectionMatrix * viewMatrix * transformMatrix * vec4(position.x, 0, position.y, 1.0);
	gl_Position = clipSpace;
	v_TexCoordinates = vec2(position.x / 2.0 + 0.5, position.y / 2.0 + 0.5) * tiling;
}

#shader fragment
#version 460

out vec4 out_Color;

in vec4 clipSpace;
in vec2 v_TexCoordinates;

uniform sampler2D reflectionTexture;
uniform sampler2D refractionTexture;
uniform sampler2D dudvMap;

uniform float moveFactor;

const float waveStrength = 0.02;

void main()
{
	vec2 normDeviceSpace = (clipSpace.xy / clipSpace.w) / 2.0 + 0.5;
	
	vec2 distortion1 = (texture(dudvMap, vec2(v_TexCoordinates.x + moveFactor, v_TexCoordinates.y)).rg * 2.0 - 1.0) * waveStrength;
	vec2 distortion2 = (texture(dudvMap, vec2(-v_TexCoordinates.x, v_TexCoordinates.y + moveFactor)).rg * 2.0 - 1.0) * waveStrength;
	vec2 totalDistortion = distortion1 + distortion2;

	vec2 refractTexCoords = vec2(normDeviceSpace.x, normDeviceSpace.y) + totalDistortion;
	refractTexCoords = clamp(refractTexCoords, 0.001, 0.999);
	
	vec2 reflectTexCoords = vec2(normDeviceSpace.x, -normDeviceSpace.y) + totalDistortion;
	reflectTexCoords.x = clamp(reflectTexCoords.x, 0.001, 0.999);
	reflectTexCoords.y = clamp(reflectTexCoords.y, -0.999, -0.001);

	vec4 refractColor = texture(refractionTexture, refractTexCoords);
	vec4 reflectColor = texture(reflectionTexture, reflectTexCoords);

	out_Color = mix(reflectColor, refractColor, 0.5);
	out_Color = mix(out_Color, vec4(0, 0.3, 0.5, 1.0), 0.2);
}