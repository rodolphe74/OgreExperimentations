varying vec3 viewPositionF;
varying vec3 normalPositionF;
varying vec4 lightPositionF;
varying vec3 cameraPositionF;

const vec3 ambiant = vec3(.0f, .0f, .0f);

void main()
{
	// shading
	// https://sites.nova.edu/mjl/graphics/lighting/flat-and-smooth-shading/

	// dFdx and dFdy return approximations to the derivatives of their arguments. 
	// Applying them to a position on a surface returns a tangent to that surface. 
	// As the cross-product of two vectors is perpendicular to both vectors, the 
	// cross-product of two tangents is normal to the surface.
	// However, because fragVertexEc is linearly interpolated across the triangle, 
	// the derivatives should be the same for all fragments, so performing the calculation 
	// for each fragment is inefficient.

	// Calculate the face normal in camera space
	// The compute the gradients, given an input value. 
	// So in this case; dfdx computes the gradient along the x-axis in view-space 
	// and dfdy calculates the gradient along the y-axis. And from these two 
	// gradient vectors, you can then calculate the normal, which should be the 
	// same for all the vertices on the triangle
	vec3 faceNormal = normalize(cross(dFdx(viewPositionF), dFdy(viewPositionF)));
	vec3 v3Light = vec3(lightPositionF.x, lightPositionF.y, lightPositionF.z);
    vec3 lightDir = normalize(v3Light - viewPositionF);
	float diff = dot(faceNormal, lightDir);
	if (diff < 0.0f) {
		diff = 0.0f;
	} else if (diff > 1.0f) {
		diff = 1.0f;
	}
	diff = 1.0f - diff;
	// if (diff > .6f) diff = 0.6f;

	vec3 F = vec3(ambiant.x + diff, ambiant.y + diff, ambiant.z + diff);
	gl_FragColor = vec4(F.xyz, 1.0);
}