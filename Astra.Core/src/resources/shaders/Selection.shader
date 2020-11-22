#shader vertex
#version 460

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 textureCoords;
layout(location = 2) in vec3 normal;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 transformMatrix;

uniform float numberOfRows;
uniform vec2 offset;

out vec2 v_TexCoordinates;

void main()
{
	vec4 worldPosition = transformMatrix * vec4(position, 1);
	vec4 positionRelativeToCam = viewMatrix * worldPosition;

	v_TexCoordinates = (textureCoords / numberOfRows) + offset;

	gl_Position = projectionMatrix * positionRelativeToCam;
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

void main()
{
	if (texture(material.diffuseMap, v_TexCoordinates).a < 0.5) { discard; }
	out_Color = vec4(0.04, 0.28, 0.26, 1.0);
}