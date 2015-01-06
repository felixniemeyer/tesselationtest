#version 430

in vec2 uv;

out vec3 color;

uniform sampler2D surface;

void main()
{
	color = vec3(1,1,1);//texture(surface, uv).rgb;
}