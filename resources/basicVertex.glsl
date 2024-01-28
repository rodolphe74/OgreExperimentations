#version 150 core

uniform vec4 lightPosition;
uniform vec3 cameraPosition;
uniform mat4 worldViewProj;
uniform vec4 v;
attribute vec4 vertex;
attribute vec4 normal;

// to fragment
varying vec3 viewPositionF;
varying vec3 normalPositionF;
varying vec4 lightPositionF;
varying vec3 cameraPositionF;

void main()
{
	vec4 viewModelPosition = worldViewProj * vertex;
	vec4 normalModelPosition = worldViewProj * normal;
	
	viewPositionF = viewModelPosition.xyz;	
	normalPositionF = normalModelPosition.xyz;

	lightPositionF = lightPosition;
	cameraPositionF = cameraPosition;

	gl_Position = viewModelPosition;
}