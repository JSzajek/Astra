#shader vertex
#version 460

layout(location = 0) in vec2 position;

uniform mat4 projectionMatrix = mat4(1.0);
uniform mat4 modelViewMatrix = mat4(1.0);

uniform vec2 texOffset1;
uniform vec2 texOffset2;
uniform vec2 texCoordInfo;

out vec2 v_TexCoordinates1;
out vec2 v_TexCoordinates2;
out float v_Blend;

void main()
{
	vec2 textureCoords = position + vec2(0.5, 0.5);
	textureCoords.y = 1.0 - textureCoords.y;
	textureCoords /= texCoordInfo.x;
	v_TexCoordinates1 = textureCoords + texOffset1;
	v_TexCoordinates2 = textureCoords + texOffset2;
	v_Blend = texCoordInfo.y;

	gl_Position = projectionMatrix * modelViewMatrix * vec4(position, 0.0, 1.0);
}

#shader fragment
#version 460

in vec2 v_TexCoordinates1;
in vec2 v_TexCoordinates2;
in float v_Blend;

out vec4 out_Color;

uniform sampler2D particle_Texture;

void main()
{
	vec4 color1 = texture(particle_Texture, v_TexCoordinates1);
	vec4 color2 = texture(particle_Texture, v_TexCoordinates2);

	out_Color = mix(color1, color2, v_Blend);
}