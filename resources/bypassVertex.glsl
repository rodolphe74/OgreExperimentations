uniform vec3 LightPosition;
uniform vec3 EyePosition;
uniform mat4 worldViewProj;	//= gl_ModelViewProjectionMatrix ?

void main() // all vertex shaders define a main() function
{
	gl_Position = worldViewProj * gl_Vertex;
}