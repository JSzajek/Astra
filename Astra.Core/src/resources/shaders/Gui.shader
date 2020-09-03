#shader vertex
#version 460

layout(location = 0) in vec2 position;

uniform mat4 transformMatrix = mat4(1.0);

out vec2 v_TexCoordinates;

void main()
{
	gl_Position = transformMatrix * vec4(position, 0.0, 1.0);
	v_TexCoordinates = vec2((position.x + 1.0) / 2.0, 1 - (position.y + 1.0) / 2.0);
}

#shader fragment
#version 460

in vec2 v_TexCoordinates;
out vec4 out_Color;
uniform sampler2D gui_Texture;

void main()
{
	out_Color = texture2D(gui_Texture, v_TexCoordinates);
}