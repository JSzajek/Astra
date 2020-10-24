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

void main()
{
	out_Color = vec4(color, texture(fontAtlas, v_TexCoordinates).a);
}