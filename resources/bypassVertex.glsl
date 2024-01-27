uniform vec3 LightPosition;
uniform vec3 EyePosition;
uniform mat4 worldViewProj;	//= gl_ModelViewProjectionMatrix ?

void main()
{
	gl_Position = worldViewProj * gl_Vertex;
}