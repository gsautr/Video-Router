/*{
	"CREDIT": "by isak.burstrom",
	"DESCRIPTION": "Shift hue while preserving luma.",
	"CATEGORIES": [
		"INKA",
		"Color Adjustment"
	],
	"INPUTS": [
		{
			"NAME": "inputImage",
			"TYPE": "image"
		},
	    {
	      "NAME" : "hue",
	      "TYPE" : "float",
	      "LABEL" : "Hue Shift"
	    },
	    {
	      "NAME" : "hue_offset",
	      "TYPE" : "float"
	    }
	]
}*/

// Converted from http://stackoverflow.com/questions/9234724/how-to-change-hue-of-a-texture-with-glsl/9234854#9234854

const vec4  kRGBToYPrime = vec4 (0.299, 0.587, 0.114, 0.0);
const vec4  kRGBToI     = vec4 (0.596, -0.275, -0.321, 0.0);
const vec4  kRGBToQ     = vec4 (0.212, -0.523, 0.311, 0.0);

const vec4  kYIQToR   = vec4 (1.0, 0.956, 0.621, 0.0);
const vec4  kYIQToG   = vec4 (1.0, -0.272, -0.647, 0.0);
const vec4  kYIQToB   = vec4 (1.0, -1.107, 1.704, 0.0);

#define PI 3.141592654

vec4 hueShift(vec4 color, float adjust) {
    // Convert to YIQ
    float   YPrime  = dot (color, kRGBToYPrime);
    float   I      = dot (color, kRGBToI);
    float   Q      = dot (color, kRGBToQ);

    // Calculate the hue and chroma
    float   hue     = atan (Q, I);
    float   chroma  = sqrt (I * I + Q * Q);

    // Make the user's adjustments
    hue += adjust;

    // Convert back to YIQ
    Q = chroma * sin (hue);
    I = chroma * cos (hue);

    // Convert back to RGB
    vec4    yIQ   = vec4 (YPrime, I, Q, 0.0);
    color.r = dot (yIQ, kYIQToR);
    color.g = dot (yIQ, kYIQToG);
    color.b = dot (yIQ, kYIQToB);
    // Save the result
	return color;
	
}
void main() {
	vec4 color = IMG_THIS_PIXEL(inputImage);
	float shift = hue * PI * 2.;
	color = hueShift(color, hue_offset * PI * 2. + shift);
	gl_FragColor = color;
}