#shader vertex
#version 330

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 textureCoords;

uniform mat4 mvpMatrix;

uniform float numberOfRows;
uniform vec2 offset;

out vec2 v_TexCoordinates;

void main()
{
	v_TexCoordinates = (textureCoords / numberOfRows) + offset;
	gl_Position = mvpMatrix * vec4(position, 1.0);
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