#shader vertex
#version 460

layout(location = 0) in vec2 position;

uniform mat4 projectionMatrix = mat4(1.0);
uniform mat4 modelViewMatrix = mat4(1.0);

void main()
{
	gl_Position = projectionMatrix * modelViewMatrix * vec4(position, 0.0, 1.0);
}

#shader fragment
#version 460

out vec4 out_Color;

void main()
{
	out_Color = vec4(1.0);
}