#shader vertex
#version 330

layout(location = 0) in vec2 position;
layout(location = 1) in mat4 transformMatrix;
layout(location = 5) in float textureIndex;

out vec2 v_TexCoordinates;
flat out uint v_TexIndex;

void main()
{
	gl_Position = transformMatrix * vec4(position, 0.0, 1.0);
	v_TexCoordinates = vec2((position.x + 1.0) / 2.0, 1 - (position.y + 1.0) / 2.0);
	v_TexIndex = uint(textureIndex);
}

#shader fragment
#version 330

#define MAX_TEXTURE_SLOTS	16

in vec2 v_TexCoordinates;
flat in uint v_TexIndex;

out vec4 out_Color;

uniform sampler2D instanced_Textures[MAX_TEXTURE_SLOTS];


void main()
{
	out_Color = texture2D(instanced_Textures[v_TexIndex], v_TexCoordinates);
}