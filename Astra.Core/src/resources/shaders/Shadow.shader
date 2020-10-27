#shader vertex
#version 460

layout(location = 0) in vec3 position;

uniform mat4 mvpMatrix;

void main()
{
	gl_Position = mvpMatrix * vec4(position, 1.0);
}

#shader fragment
#version 460

uniform sampler2D u_Texture;

out vec4 out_Color;

void main()
{
	out_Color = vec4(1);
}