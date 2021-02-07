#shader vertex
#version 330

layout(location = 0) in vec2 position;

out vec2 v_TexCoordinates;

void main()
{
	gl_Position = vec4(position, 0.0, 1.0);
	v_TexCoordinates = position * 0.5 + 0.5;
}

#shader fragment
#version 330

out vec4 out_Color;

in vec2 v_TexCoordinates;

uniform sampler2D colorMap;
uniform sampler2D brightBlur;

void main()
{
	vec3 hdrColor = texture2D(colorMap, v_TexCoordinates).rgb;
	vec3 brightColor = texture2D(brightBlur, v_TexCoordinates).rgb;

	out_Color = vec4(hdrColor + brightColor, 1.0);
}