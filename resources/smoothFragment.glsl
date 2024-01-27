varying vec3 viewPositionF;
varying vec3 normalPositionF;
varying vec4 lightPositionF;
varying vec3 cameraPositionF;

const vec3 ambiant = vec3(.3, .3, .4);

void main()
{
	// shading
	// https://sites.nova.edu/mjl/graphics/lighting/flat-and-smooth-shading/

	vec3 v3Light = vec3(lightPositionF.x, lightPositionF.y, lightPositionF.z);
    vec3 lightDir = normalize(v3Light - viewPositionF);
	
	float diff = dot(normalPositionF, lightDir);
	if (diff < 0.0f) {
		diff = 0.0f;
	} else if (diff > 1.0f) {
		diff = 1.0f;
	}

	vec3 F = vec3(ambiant.x + diff, ambiant.y + diff, ambiant.z + diff);
	gl_FragColor = vec4(F.xyz, 1.0);
}