/*
 *
 *  Peter R. Elespuru
 *  
 *  modified from this idea:
 *  http://r3dux.org/2011/06/glsl-image-processing/
 */

uniform sampler2D img;
varying vec2 texCoord;

void main(void) {
		float grey = dot(texture2D(img, texCoord).rgb, vec3(0.299, 0.587, 0.114));
		gl_FragColor = vec4(grey, grey, grey, 1.0);	
		
}
