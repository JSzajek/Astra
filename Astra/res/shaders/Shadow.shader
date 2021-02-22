#shader vertex
#version 330

#define MAX_BONES			75
#define MAX_BONE_INFLUENCE	3

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 textureCoords;
layout(location = 3) in ivec3 boneIds;
layout(location = 4) in vec3 weights;

uniform mat4 mvpMatrix;

uniform float numberOfRows;
uniform vec2 offset;

uniform int animated = 0;
uniform mat4 boneTransformation[MAX_BONES];

out vec2 v_TexCoordinates;

void CalcBoneInfluence(out vec4 totalPosition);

void main()
{
	vec4 aPosition = vec4(position, 1.0f);
	if (animated > 0)
	{
		vec4 totalPosition = vec4(0);
		CalcBoneInfluence(totalPosition);
		aPosition = totalPosition;
	}

	v_TexCoordinates = (textureCoords / numberOfRows) + offset;
	gl_Position = mvpMatrix * aPosition;
}

void CalcBoneInfluence(out vec4 totalPosition)
{
	vec4 partialPos = vec4(position, 1.0f);
	for (int i = 0; i < MAX_BONE_INFLUENCE; i++)
	{
		if (boneIds[i] < 0)
			continue;
		totalPosition += boneTransformation[boneIds[i]] * partialPos * weights[i];
	}
}

#shader fragment
#version 330

in vec2 v_TexCoordinates;

uniform sampler2D u_Texture;

out vec4 out_Color;

void main()
{
	float alpha = texture(u_Texture, v_TexCoordinates).a;
	if (alpha < 0.5) { discard; }
	
	out_Color = vec4(1);
}