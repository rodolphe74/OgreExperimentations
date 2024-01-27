uniform vec3 lightPosition;
uniform vec3 eyePosition;
uniform mat4 worldViewProj;	//= gl_ModelViewProjectionMatrix ?

void main()
{
	gl_Position = worldViewProj * gl_Vertex;
}