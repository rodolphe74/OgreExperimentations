#version 150 core

varying vec3 viewPositionF;
varying vec3 normalPositionF;
varying vec4 lightPositionF;
varying vec3 cameraPositionF;

const vec3 ambiant = vec3(.0f, .0f, .0f);
const float darkening = 0.9f;
const int sz = 6;
const float dv = 9.0f;

// https://github.com/ImageMagick/ImageMagick/blob/be0f61a0d4ca4852a29cf08f74e7790803d649ac/config/thresholds.xml#L132
const int indexMatrix4x4d9[16] = int[](
	4,  2,  7,  5,
	3,  1,  8,  6,
	7,  5,  4,  2,
	8,  6,  3,  1);

const int indexMatrix6x6d9[36] = int[](
	14, 13, 10,  8,  2,  3,
	16, 18, 12,  7,  1,  4,
	15, 17, 11,  9,  6,  5,
	8,  2,  3, 14, 13, 10,
	7,  1,  4, 16, 18, 12,
	9,  6,  5, 15, 17, 11);

const int indexMatrix8x8d9[64] = int[](
	13,  7,  8, 14, 17, 21, 22, 18,
	6,  1,  3,  9, 28, 31, 29, 23,
	5,  2,  4, 10, 27, 32, 30, 24,
	16, 12, 11, 15, 20, 26, 25, 19,
	17, 21, 22, 18, 13,  7,  8, 14,
	28, 31, 29, 23,  6,  1,  3,  9,
	27, 32, 30, 24,  5,  2,  4, 10,
	20, 26, 25, 19, 16, 12, 11, 15);


vec3 hsl2rgb( in vec3 c )
{
    vec3 rgb = clamp( abs(mod(c.x*6.0+vec3(0.0,4.0,2.0),6.0)-3.0)-1.0, 0.0, 1.0 );

    return c.z + c.y * (rgb-0.5)*(1.0-abs(2.0*c.z-1.0));
}

vec3 rgb2hsl( in vec3 c ){
  float h = 0.0;
	float s = 0.0;
	float l = 0.0;
	float r = c.r;
	float g = c.g;
	float b = c.b;
	float cMin = min( r, min( g, b ) );
	float cMax = max( r, max( g, b ) );

	l = ( cMax + cMin ) / 2.0;
	if ( cMax > cMin ) {
		float cDelta = cMax - cMin;
        
        //s = l < .05 ? cDelta / ( cMax + cMin ) : cDelta / ( 2.0 - ( cMax + cMin ) ); Original
		s = l < .0 ? cDelta / ( cMax + cMin ) : cDelta / ( 2.0 - ( cMax + cMin ) );
        
		if ( r == cMax ) {
			h = ( g - b ) / cDelta;
		} else if ( g == cMax ) {
			h = 2.0 + ( b - r ) / cDelta;
		} else {
			h = 4.0 + ( r - g ) / cDelta;
		}

		if ( h < 0.0) {
			h += 6.0;
		}
		h = h / 6.0;
	}
	return vec3( h, s, l );
}


const int indexMatrix4x4[16] = int[](
	0,  8,  2,  10,
	12, 4,  14, 6,
	3,  11, 1,  9,
	15, 7,  13, 5);


float indexValue()
{
    int x = int(mod(gl_FragCoord.x, sz));
    int y = int(mod(gl_FragCoord.y, sz));
	// return indexMatrix4x4d9[(x + y * sz)] / dv;
	return indexMatrix6x6d9[(x + y * sz)] / dv;
	// return indexMatrix8x8d9[(x + y * sz)] / dv;
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
	vec3 hsl = rgb2hsl(F);

	hsl.z = hsl.z - (hsl.z * darkening);
	vec3 darker = hsl2rgb(hsl);
	float color = getGrey(vec4(darker, 1.0));

	gl_FragColor = vec4(vec3(dither(color)), 1);
}