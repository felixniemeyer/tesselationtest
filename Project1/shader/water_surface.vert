#version 430

layout(location=0) in vec3 vertex;
layout(location=1) in vec2 texCoord;

out vec3 vPosition;
out vec2 uv;

void main(){
	vPosition = vertex;
	uv = texCoord;
}