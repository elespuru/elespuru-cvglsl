/*
 *
 *  Peter R. Elespuru
 *  
 *  modified from:
 *
 * halftone fragment shader
 *
 * Copyright (c) 2008 Tobias Rautenkranz <tobias@rautenkranz.ch>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

/* Texture to halftone. */
uniform sampler2D img;
 
void main(void) {
	ivec2 halftoneResolution = ivec2(600, 600);
	
	float angle = -45.0;
	float contrastDelta = 0.3; // higher -> grey gets darker
	float brightness = 0.0; // analog for white
	float blackness = 1.1; // higher -> larger areas completely covered by dots
	float smooth = 0.2;
	vec4 dotColor = vec4(0.0, 0.0, 0.0, 1.0);
	vec4 backgroundColor = vec4(1.0, 1.0, 1.0, 0.0);
	
	mat2 rotate = mat2(cos(angle), -sin(angle),
	                         sin(angle), cos(angle));
	
	mat2 inverse_rotate = mat2(cos(angle), sin(angle),
	                                -sin(angle), cos(angle));

	/* Distance to next dot divided by two. */ 
	vec2 halfLineDist = vec2(1.0)/vec2(halftoneResolution)/vec2(2.0);

	/* Find center of the halftone dot. */
	vec2 center =  rotate * gl_TexCoord[0].st;
	center = floor(center * vec2(halftoneResolution)) / vec2(halftoneResolution);
	center += halfLineDist;
	center = inverse_rotate * center;

	/* Only red (texture is grey scale) */
	float luminance = texture2D(img, center).r;


	/* Radius of the halftone dot. */
	float radius = sqrt(2.0)*halfLineDist.x*(1.0 - luminance)*blackness;

	float contrast = 1.0 + (contrastDelta)/(2.0);
	float radiusSqrd = contrast * pow(radius,2.0)
		- (contrastDelta * halfLineDist.x*halfLineDist.x)/2.0
		- brightness * halfLineDist.x*halfLineDist.x;


	vec2 power = pow(abs(center-gl_TexCoord[0].st),vec2(2.0));
	float pixelDist2 = power.x + power.y; // Distance pixel to center squared.

	float delta = smooth*pow(halfLineDist.x,2.0);
	float gradient = smoothstep(radiusSqrd-delta, radiusSqrd+delta, pixelDist2);
	gl_FragColor = mix(dotColor, backgroundColor, gradient);
}
