#version 150 core

varying vec3 viewPositionF;
varying vec3 normalPositionF;
varying vec4 lightPositionF;
varying vec3 cameraPositionF;

const vec3 ambiant = vec3(.0f, .0f, .0f);

const int indexMatrix4x4[16] = int[](
	0,  8,  2,  10,
	12, 4,  14, 6,
	3,  11, 1,  9,
	15, 7,  13, 5);

float indexValue()
{
    int x = int(mod(gl_FragCoord.x, 4));
    int y = int(mod(gl_FragCoord.y, 4));
    return indexMatrix4x4[(x + y * 4)] / 16.0;
}

float dither(float color)
{
	// http://alex-charlton.com/posts/Dithering_on_the_GPU/
    float closestColor = (color < 0.5) ? 0 : 1;
    float secondClosestColor = 1 - closestColor;
    float d = indexValue();
    float distance = abs(closestColor - color);
    return (distance < d) ? closestColor : secondClosestColor;
}

float getGrey(vec4 color)
{
	return 0.2126 * color.x + 0.7152 * color.y + 0.0722 * color.z;
}

void main()
{
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

	vec3 F = vec3(ambiant.x + diff, ambiant.y + diff, ambiant.z + diff);
	float color = getGrey(vec4(F.xyz, 1.0));
	gl_FragColor = vec4(vec3(dither(color)), 1);
}