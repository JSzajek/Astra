#shader vertex
#version 330

layout(location = 0) in vec4 vertex; // <vec2 pos, vec2 tex>

out vec2 v_TexCoordinates;

uniform mat4 transformMatrix;
uniform mat4 projectionMatrix;

void main()
{
	gl_Position = projectionMatrix * transformMatrix * vec4(vertex.xy, 0.0, 1.0);
	v_TexCoordinates = vertex.zw;
}

#shader fragment
#version 330

in vec2 v_TexCoordinates;
in vec4 v_modulateColor;

out vec4 out_Color;

uniform sampler2D fontAtlas;
uniform vec4 modulateColor;

void main()
{
	out_Color = vec4(1.0, 1.0, 1.0, texture(fontAtlas, v_TexCoordinates).r);
	out_Color *= modulateColor;
}