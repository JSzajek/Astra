#shader vertex
#version 330

layout(location = 0) in vec2 position;
layout(location = 1) in mat4 modelViewMatrix;
layout(location = 5) in vec4 texOffsets;
layout(location = 6) in vec3 texCoordInfo;

uniform mat4 projectionMatrix = mat4(1.0);

out vec2 v_TexCoordinates1;
out vec2 v_TexCoordinates2;
out float v_Blend;
out float v_TextureIndex;

void main()
{
	vec2 textureCoords = position + vec2(0.5, 0.5);
	textureCoords.y = 1.0 - textureCoords.y;
	textureCoords /= texCoordInfo.y;
	v_TexCoordinates1 = textureCoords + texOffsets.xy;
	v_TexCoordinates2 = textureCoords + texOffsets.zw;
	v_Blend = texCoordInfo.z;
	v_TextureIndex = int(texCoordInfo.x);

	gl_Position = projectionMatrix * modelViewMatrix * vec4(position, 0.0, 1.0);
}

#shader fragment
#version 330

#define MAX_TEXTURE_SLOTS	16

in vec2 v_TexCoordinates1;
in vec2 v_TexCoordinates2;
in float v_Blend;
in float v_TextureIndex;

out vec4 out_Color;

uniform sampler2D instanced_Textures[MAX_TEXTURE_SLOTS];

void main()
{
	vec4 color1 = texture(instanced_Textures[int(v_TextureIndex)], v_TexCoordinates1);
	vec4 color2 = texture(instanced_Textures[int(v_TextureIndex)], v_TexCoordinates2);

	out_Color = mix(color1, color2, v_Blend);
}