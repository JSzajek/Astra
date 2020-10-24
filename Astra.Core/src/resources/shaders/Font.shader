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
uniform float width;
uniform float edge;

uniform vec3 outlineColor;
uniform float outlineWidth;
uniform float outlineEdge;

uniform vec2 offset;

void main()
{
	float distance = 1.0 - texture(fontAtlas, v_TexCoordinates).a;
	float alpha = 1.0 - smoothstep(width, width + edge, distance);

	if (outlineWidth == outlineEdge)
	{
		out_Color = vec4(color, alpha);
	}
	else
	{
		float outlineDist = 1.0 - texture(fontAtlas, v_TexCoordinates + offset).a;
		float outlineAlpha = 1.0 - smoothstep(outlineWidth, outlineWidth + outlineEdge, outlineDist);
		
		float overallAlpha = alpha + (1.0 - alpha) * outlineAlpha;
		out_Color = vec4(mix(outlineColor, color, alpha / overallAlpha), overallAlpha);
	}
}