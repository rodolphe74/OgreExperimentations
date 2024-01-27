varying vec3 viewPosition;

void main()
{
	// Calculate the face normal in camera space
	// The compute the gradients, given an input value. 
	// So in this case; dfdx computes the gradient along the x-axis in view-space 
	// and dfdy calculates the gradient along the y-axis. And from these two 
	// gradient vectors, you can then calculate the normal, which should be the 
	// same for all the vertices on the triangle
	vec3 normalCs = normalize(cross(dFdx(viewPosition), dFdy(viewPosition)));

	gl_FragColor = vec4(normalCs, 1.0); 
}