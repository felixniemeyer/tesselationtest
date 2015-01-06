#version 430

layout(location=0) in vec3 vertex;
layout(location=1) in vec2 texCoord;

out vec2 uv;

uniform mat4 mvp;

void main(){
	gl_Position.xyz = vertex;
	gl_Position.w = 1.0;
	gl_Position = mvp * gl_Position;
	uv = texCoord;
}