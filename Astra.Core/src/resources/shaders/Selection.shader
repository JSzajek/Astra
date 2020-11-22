#shader vertex
#version 460

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
	v_TexCoordinates = (textureCoords / numberOfRows) + offset;
	gl_Position = projectionMatrix * viewMatrix * transformMatrix * vec4(position, 1);
}

#shader fragment
#version 460

in vec2 v_TexCoordinates;
out vec4 out_Color;

struct Material
{
	sampler2D diffuseMap;
};
uniform Material material;

uniform vec3 selectionColor;

void main()
{
	if (texture(material.diffuseMap, v_TexCoordinates).a < 0.5) { discard; }
	out_Color = vec4(selectionColor, 1.0);
}