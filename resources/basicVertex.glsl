uniform vec3 LightPosition;
uniform vec3 EyePosition;
uniform mat4 worldViewProj;	//= gl_ModelViewProjectionMatrix ?

// to fragment
varying vec3 viewPosition;

void main()
{
	vec4 viewModelPosition = worldViewProj * gl_Vertex;
	viewPosition = viewModelPosition.xyz;
	gl_Position = viewModelPosition;
}