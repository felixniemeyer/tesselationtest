#version 430

in vec2 uv;

out vec3 color;

uniform sampler2D surface;

void main()
{
	color = texture(surface, uv).rgb;
}