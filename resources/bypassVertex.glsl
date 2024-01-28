#version 150 core

uniform vec3 lightPosition;
uniform vec3 eyePosition;
uniform mat4 worldViewProj;

attribute vec4 vertex;

void main()
{
	gl_Position = worldViewProj * vertex;
}