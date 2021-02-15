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

uniform sampler2D gui_Texture;

const float contrast = 0.3;

void main()
{
	out_Color = texture2D(gui_Texture, v_TexCoordinates);
	out_Color.rgb = (out_Color.rgb - 0.5) * (1.0 + contrast) + 0.5;
}