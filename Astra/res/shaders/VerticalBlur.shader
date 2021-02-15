#shader vertex
#version 330

layout(location = 0) in vec2 position;

out vec2 v_TexCoordinates[11];

uniform float targetHeight;

void main()
{
	gl_Position = vec4(position, 0.0, 1.0);
	vec2 centerTexCoord = position * 0.5 + 0.5;

	float pixelSize = 1.0 / targetHeight;
	for (int i = -5; i <= 5; i++)
	{
		v_TexCoordinates[i + 5] = centerTexCoord + vec2(0, pixelSize * i);
	}
}

#shader fragment
#version 330

in vec2 v_TexCoordinates[11];

out vec4 out_Color;

uniform sampler2D gui_Texture;

void main()
{
	out_Color = vec4(0);
	out_Color += texture2D(gui_Texture, v_TexCoordinates[0]) * 0.0093;
	out_Color += texture2D(gui_Texture, v_TexCoordinates[1]) * 0.028002;
	out_Color += texture2D(gui_Texture, v_TexCoordinates[2]) * 0.065984;
	out_Color += texture2D(gui_Texture, v_TexCoordinates[3]) * 0.121703;
	out_Color += texture2D(gui_Texture, v_TexCoordinates[4]) * 0.175713;
	out_Color += texture2D(gui_Texture, v_TexCoordinates[5]) * 0.198596;
	out_Color += texture2D(gui_Texture, v_TexCoordinates[6]) * 0.175713;
	out_Color += texture2D(gui_Texture, v_TexCoordinates[7]) * 0.121703;
	out_Color += texture2D(gui_Texture, v_TexCoordinates[8]) * 0.065984;
	out_Color += texture2D(gui_Texture, v_TexCoordinates[9]) * 0.028002;
	out_Color += texture2D(gui_Texture, v_TexCoordinates[10]) * 0.0093;
}