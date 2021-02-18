#shader vertex
#version 330

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 textureCoords;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 transformMatrix;

uniform float numberOfRows;
uniform vec2 offset;

out vec2 v_TexCoordinates;

void main()
{
	vec4 worldPosition = transformMatrix * vec4(position, 1);
	v_TexCoordinates = (textureCoords / numberOfRows) + offset;
	gl_Position = projectionMatrix * viewMatrix * worldPosition;
}

#shader fragment
#version 330

in vec2 v_TexCoordinates;

out vec4 out_Color;

struct Material
{
	sampler2D diffuseMap;
};
uniform Material material;

void main()
{
	out_Color = texture(material.diffuseMap, v_TexCoordinates);
	if (out_Color.a < 0.5) { discard; }
}