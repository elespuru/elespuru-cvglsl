/*
 *
 *  Peter R. Elespuru
 *  
 *  modified from:
 *  http://r3dux.org/2011/06/glsl-image-processing/
 */

uniform sampler2D img;
varying vec2 texCoord;

void main(void) {
	vec4 texMapColour = texture2D(img, texCoord);
	gl_FragColor = vec4(1.0 - texMapColour.rgb, 1.0);
}