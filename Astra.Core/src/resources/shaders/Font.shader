#shader vertex
#version 460

layout(location = 0) in vec2 position;
layout(location = 1) in vec2 textureCoords;

uniform vec2 translation;

out vec2 v_TexCoordinates;

void main()
{
	gl_Position = vec4(position + translation * vec2(2.0, -2.0), 0.0, 1.0);
	v_TexCoordinates = textureCoords;
}

#shader fragment
#version 460

in vec2 v_TexCoordinates;

out vec4 out_Color;

uniform sampler2D fontAtlas;
uniform vec3 color;

const float width = 0.5;
const float edge = 0.1;

const float borderWidth = 0.0;
const float borderEdge = 0.4;

const vec2 offset = vec2(0, 0);

const vec3 outlineColor = vec3(0.2, 0.2, 0.2);

void main()
{
	float distance = 1.0 - texture(fontAtlas, v_TexCoordinates).a;
	float alpha = 1.0 - smoothstep(width, width + edge, distance);

	float distance2 = 1.0 - texture(fontAtlas, v_TexCoordinates + offset).a;
	float outlineAlpha = 1.0 - smoothstep(borderWidth, borderWidth + borderEdge, distance2);

	float overallAlpha = alpha + (1.0 - alpha) * outlineAlpha;
	vec3 overallColor = mix(outlineColor, color, alpha / overallAlpha);
	out_Color = vec4(overallColor, overallAlpha);
}