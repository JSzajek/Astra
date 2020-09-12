#shader vertex
#version 460

layout(location = 0) in vec2 position;

uniform mat4 projectionMatrix = mat4(1.0);
uniform mat4 viewMatrix = mat4(1.0);
uniform mat4 transformMatrix = mat4(1.0);

out vec2 v_TexCoordinates;

void main()
{
	gl_Position = projectionMatrix * viewMatrix * transformMatrix * vec4(position.x, 0, position.y, 1.0);
	v_TexCoordinates = vec2(position.x / 2.0 + 0.5, position.y / 2.0 + 0.5);
}

#shader fragment
#version 460

in vec2 v_TexCoordinates;

out vec4 out_Color;

uniform sampler2D u_Texture;

void main()
{
	out_Color = vec4(0,0,1,1);
}