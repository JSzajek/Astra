#shader vertex
#version 460

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 textureCoords;
layout(location = 2) in vec3 normal;

uniform mat4 projectionMatrix = mat4(1.0);
uniform mat4 viewMatrix = mat4(1.0);
uniform mat4 transformMatrix = mat4(1.0);

out vec2 v_TexCoordinates;

void main()
{
	vec4 worldPosition = transformMatrix * vec4(position, 1);
	v_TexCoordinates = textureCoords;
	gl_Position = projectionMatrix * viewMatrix * worldPosition;
}

#shader fragment
#version 460

in vec2 v_TexCoordinates;
out vec4 out_Color;

uniform sampler2D u_Texture;

void main()
{
	out_Color = texture2D(u_Texture, v_TexCoordinates);
}