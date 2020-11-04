#shader vertex
#version 460

layout(location = 0) in vec2 position;

uniform mat4 projectionMatrix = mat4(1.0);
uniform mat4 modelViewMatrix = mat4(1.0);

out vec2 v_TexCoords;

void main()
{
	v_TexCoords = position + vec2(0.5, 0.5);
	v_TexCoords.y = 1.0 - v_TexCoords.y;

	gl_Position = projectionMatrix * modelViewMatrix * vec4(position, 0.0, 1.0);
}

#shader fragment
#version 460

in vec2 v_TexCoords;

out vec4 out_Color;

uniform sampler2D gizmo_Texture;

void main()
{
	vec4 textureColor = texture(gizmo_Texture, v_TexCoords);
	out_Color = textureColor;
}