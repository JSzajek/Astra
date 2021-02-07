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
out vec4 color;

uniform sampler2D text;
uniform vec3 textColor;

void main()
{
	vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, v_TexCoordinates).r);
	color = vec4(textColor, 1.0) * sampled;
}