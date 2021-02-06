#shader vertex
#version 330

layout(location = 0) in vec4 vertex; // <vec2 pos, vec2 tex>

out vec2 v_TexCoordinates;

uniform mat4 transformMatrix;
uniform mat4 projectionMatrix;

void main()
{
	gl_Position = projectionMatrix * transformMatrix * vec4(vertex.xy, 0.0, 1.0);
	v_TexCoordinates = vertex.zw;
}

#shader fragment
#version 330

in vec2 v_TexCoordinates;
out vec4 out_Color;

uniform sampler2D fontAtlas;
uniform vec3 textColor;
//uniform sampler2D fontAtlas;
//
//uniform vec3 color;
//uniform float width;
//uniform float edge;
//
//uniform vec3 outlineColor;
//uniform float outlineWidth;
//uniform float outlineEdge;
//
//uniform vec2 offset;

void main()
{
	vec4 sampled = vec4(1.0, 1.0, 1.0, texture(fontAtlas, v_TexCoordinates).r);
	out_Color = vec4(textColor, 1.0) * sampled;

	/*float distance = 1.0 - texture(fontAtlas, v_TexCoordinates).a;
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
	}*/
}