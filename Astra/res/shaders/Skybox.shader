#shader vertex
#version 330

layout(location = 0) in vec3 position;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

out vec3 v_TexCoordinates;

void main()
{
	v_TexCoordinates = position;
	gl_Position = (projectionMatrix * viewMatrix * vec4(position, 1.0)).xyww;
}

#shader fragment
#version 330

in vec3 v_TexCoordinates;

const float lowerLimit = 0.0f;
const float upperLimit = 30.0f;

out vec4 out_Color;

uniform vec3 fogColor;
uniform samplerCube cubeMap;
uniform samplerCube cubeMap2;
uniform float blendFactor;

void main()
{
	vec4 texture1 = texture(cubeMap, v_TexCoordinates);
	vec4 texture2 = texture(cubeMap2, v_TexCoordinates);
	vec4 finalColor = mix(texture1, texture2, blendFactor);
	float factor = (v_TexCoordinates.y - lowerLimit) / (upperLimit - lowerLimit);
	factor = clamp(factor, 0.0, 1.0);
	out_Color = mix(vec4(fogColor, 1.0), finalColor, factor);
}