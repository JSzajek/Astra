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

in vec2 v_TexCoordinates;

out vec4 out_Color;

uniform sampler2D hdrBuffer;
uniform float exposure;
uniform int hdrActive;

const float gamma = 2.2;

void main()
{
	vec3 hdrColor = texture(hdrBuffer, v_TexCoordinates).rgb;
	if (hdrActive > 0)
	{
		vec3 result = vec3(1.0) - exp(-hdrColor * exposure);
		result = pow(result, vec3(1.0 / gamma));
		out_Color = vec4(result, 1.0);
	}
	else
	{
		vec3 result = pow(hdrColor, vec3(1.0 / gamma));
		out_Color = vec4(hdrColor, 1.0);
	}
}	