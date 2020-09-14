#shader vertex
#version 460

layout(location = 0) in vec2 position;

out vec4 clipSpace;

uniform mat4 projectionMatrix = mat4(1.0);
uniform mat4 viewMatrix = mat4(1.0);
uniform mat4 transformMatrix = mat4(1.0);

void main()
{
	clipSpace = projectionMatrix * viewMatrix * transformMatrix * vec4(position.x, 0, position.y, 1.0);
	gl_Position = clipSpace;
}

#shader fragment
#version 460

out vec4 out_Color;

in vec4 clipSpace;

uniform sampler2D reflectionTexture;
uniform sampler2D refractionTexture;

void main()
{
	vec2 normDeviceSpace = (clipSpace.xy / clipSpace.w) / 2.0 + 0.5;
	
	vec4 refractColor = texture(refractionTexture, normDeviceSpace);
	vec4 reflectColor = texture(reflectionTexture, vec2(normDeviceSpace.x, -normDeviceSpace.y));

	out_Color = mix(reflectColor, refractColor, 0.5);
}