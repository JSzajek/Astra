#shader vertex
#version 460

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 textureCoords;
layout(location = 2) in vec3 normal;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 transformMatrix;

out vec2 v_TexCoordinates;

void main()
{
	v_TexCoordinates = textureCoords * 15;
	gl_Position = projectionMatrix * viewMatrix * transformMatrix * vec4(position, 1);
}

#shader fragment
#version 460

in vec2 v_TexCoordinates;

out vec4 out_Color;

uniform sampler2D u_Texture;

void main()
{
	vec4 textureColor = texture2D(u_Texture, v_TexCoordinates);
	if (textureColor.a < 0.5) { discard; }

	out_Color = textureColor;
}