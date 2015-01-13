#version 430

layout(location=0) in vec3 vertex;
layout(location=1) in vec2 texCoord;

out vec4 vPosition;
out vec2 vUV;

void main(){
	vPosition = vec4( vertex, 1 );
	vUV = texCoord;
}