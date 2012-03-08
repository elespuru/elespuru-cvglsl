/*
 *
 *  Peter R. Elespuru
 *  Derived from Shader by: Jason Gorski
 *  Email: jasejc 'at' aol.com
 *
 *  LED Shader Tutorial
 *  For more information about this shader view the tutorial page
 *  at http://www.lighthouse3d.com/opengl/ledshader or email me
 * 
 */

#define KERNEL_SIZE 9

uniform sampler2D img; //texure to be applied to billboard quad
vec2 texCoords[KERNEL_SIZE]; //stores texture lookup offsets from a base case

vec4 applyLuminanceStepping(in vec4 color) {

	int luminanceSteps = 16;
	float luminanceBoost = 0.0;

	float sum = color.r + color.g + color.b;
	float luminance = sum/3.0; //brightness or luminance of color
	vec3 ratios = vec3(color.r/luminance, color.g/luminance, color.b/luminance); //ratio stores each channel's contribution to the luminance

	float luminanceStep = 1.0/float(luminanceSteps); //how big each luminance bin is
	float luminanceBin = ceil(luminance/luminanceStep); //figure out which bin the color is in
	float luminanceFactor = luminanceStep * luminanceBin + luminanceBoost; //store the luminance of the color we are making including luminanceBoost

	return vec4(ratios * luminanceFactor,1.0); //use ratios * luminanceFactor as our new color so that original color hue is maintained
}

void main(void) {
	
	float tolerance = 0.1;
	float pixelRadius = 0.45;
	int pixelSize = 5;
	ivec2 billboardSize = ivec2(600,600);

	int luminanceSteps = 16;
	float luminanceBoost = 0.0;
	
	vec4 avgColor; //will hold our averaged color from our sample points
	vec2 texCoordsStep = 1.0/(vec2(float(billboardSize.x),float(billboardSize.y))/float(pixelSize)); //width of "pixel region" in texture coords
	vec2 pixelRegionCoords = fract(gl_TexCoord[0].st/texCoordsStep); //x and y coordinates within "pixel region"
	vec2 pixelBin = floor(gl_TexCoord[0].st/texCoordsStep); //"pixel region" number counting away from base case
	vec2 inPixelStep = texCoordsStep/3.0; //width of "pixel region" divided by 3 (for KERNEL_SIZE = 9, 3x3 square)
	vec2 inPixelHalfStep = inPixelStep/2.0;


	//use offset (pixelBin * texCoordsStep) from base case (the lower left corner of billboard) to compute texCoords
	texCoords[0] = vec2(inPixelHalfStep.x, inPixelStep.y*2.0 + inPixelHalfStep.y) + pixelBin * texCoordsStep;
	texCoords[1] = vec2(inPixelStep.x + inPixelHalfStep.x, inPixelStep.y*2.0 + inPixelHalfStep.y) + pixelBin * texCoordsStep;
	texCoords[2] = vec2(inPixelStep.x*2.0 + inPixelHalfStep.x, inPixelStep.y*2.0 + inPixelHalfStep.y) + pixelBin * texCoordsStep;
	texCoords[3] = vec2(inPixelHalfStep.x, inPixelStep.y + inPixelHalfStep.y) + pixelBin * texCoordsStep;
	texCoords[4] = vec2(inPixelStep.x + inPixelHalfStep.x, inPixelStep.y + inPixelHalfStep.y) + pixelBin * texCoordsStep;
	texCoords[5] = vec2(inPixelStep.x*2.0 + inPixelHalfStep.x, inPixelStep.y + inPixelHalfStep.y) + pixelBin * texCoordsStep;
	texCoords[6] = vec2(inPixelHalfStep.x, inPixelHalfStep.y) + pixelBin * texCoordsStep;
	texCoords[7] = vec2(inPixelStep.x + inPixelHalfStep.x, inPixelHalfStep.y) + pixelBin * texCoordsStep;
	texCoords[8] = vec2(inPixelStep.x*2.0 + inPixelHalfStep.x, inPixelHalfStep.y) + pixelBin * texCoordsStep;

	//take average of 9 pixel samples
	avgColor = texture2D(img, texCoords[0]) +
				texture2D(img, texCoords[1]) +
				texture2D(img, texCoords[2]) +
				texture2D(img, texCoords[3]) +
				texture2D(img, texCoords[4]) +
				texture2D(img, texCoords[5]) +
				texture2D(img, texCoords[6]) +
				texture2D(img, texCoords[7]) +
				texture2D(img, texCoords[8]);

	avgColor /= float(KERNEL_SIZE);

	//get a new color with the discretized luminance value
	avgColor = applyLuminanceStepping(avgColor);

	//blend between fragments in the circle and out of the circle defining our "pixel region"
	//Equation of a circle: (x - h)^2 + (y - k)^2 = r^2
	vec2 powers = pow(abs(pixelRegionCoords - 0.5),vec2(2.0));

	float radiusSqrd = pow(pixelRadius,2.0);
	float gradient = smoothstep(radiusSqrd-tolerance, radiusSqrd+tolerance, powers.x+powers.y);
	gl_FragColor = mix(avgColor, vec4(0.1,0.1,0.1,1.0), gradient);

}
