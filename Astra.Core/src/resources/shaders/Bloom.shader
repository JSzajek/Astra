#shader vertex
#version 460

layout(location = 0) in vec2 position;

out vec2 v_TexCoordinates;

void main()
{
	gl_Position = vec4(position, 0.0, 1.0);
	v_TexCoordinates = position * 0.5 + 0.5;
}

#shader fragment
#version 460

layout(location = 0) out vec4 out_Color;
layout(location = 1) out vec4 bright_Color;

in vec2 v_TexCoordinates;

uniform sampler2D gui_Texture;

void main()
{
	out_Color = texture2D(gui_Texture, v_TexCoordinates);

	float brightness = dot(out_Color.rgb, vec3(0.4126, 0.7152, 0.0722));
	if (brightness > 1.0)
	{
		bright_Color = out_Color;
	}
	else
	{
		bright_Color = vec4(0, 0, 0, 1);
	}
}