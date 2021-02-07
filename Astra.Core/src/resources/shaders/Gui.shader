#shader vertex
#version 330

layout(location = 0) in vec4 vertex; // <vec2 position, vec2 texCoords>

layout(location = 1) in mat4 transformMatrix;
layout(location = 5) in vec4 textureInfo;
layout(location = 6) in vec4 modulateColor;

uniform mat4 projectionMatrix;

out vec2 v_TexCoordinates;
flat out uint v_TexIndex;

out vec4 v_modulateColor;

void main()
{
	gl_Position = projectionMatrix * transformMatrix * vec4(vertex.xy, 0.0, 1.0);
	
	v_TexIndex = uint(textureInfo.x);
	v_TexCoordinates = vertex.zw;
	v_TexCoordinates = (vec2(vertex.z, vertex.w) / textureInfo.y) + textureInfo.zw;
	
	v_modulateColor = modulateColor;
}

#shader fragment
#version 330

#define MAX_TEXTURE_SLOTS	16

in vec2 v_TexCoordinates;
in vec4 v_modulateColor;

flat in uint v_TexIndex;

out vec4 out_Color;

uniform sampler2D instanced_Textures[MAX_TEXTURE_SLOTS];

void main()
{
	out_Color = texture2D(instanced_Textures[v_TexIndex], v_TexCoordinates);
	if (out_Color.a < 0.5) { discard; }
	out_Color *= v_modulateColor;
}