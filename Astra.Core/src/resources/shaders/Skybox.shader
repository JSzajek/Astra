#shader vertex
#version 460

layout(location = 0) in vec3 position;

uniform mat4 projectionMatrix = mat4(1.0);
uniform mat4 viewMatrix = mat4(1.0);

out vec3 v_TexCoordinates;

void main()
{
	gl_Position = projectionMatrix * viewMatrix * vec4(position, 1.0);
	v_TexCoordinates = position;
}

#shader fragment
#version 460

in vec3 v_TexCoordinates;

out vec4 out_Color;
uniform samplerCube cubeMap;

void main()
{
	out_Color = texture(cubeMap, v_TexCoordinates);
}