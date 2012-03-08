/*
 *
 *  Peter R. Elespuru
 *
 *	modified from this idea:
 *	http://learningwebgl.com/blog/?p=2858
 *
 */

uniform sampler2D Texture;

void main()
{
	float lum = length(texture2D(Texture, gl_TexCoord[0].xy).rgb);
	
	gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);
	
	if (lum < 1.00) {
		if (mod(gl_FragCoord.x + gl_FragCoord.y, 10.0) == 0.0) {
			gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);
		}
	}
	
	if (lum < 0.75) {
		if (mod(gl_FragCoord.x - gl_FragCoord.y, 10.0) == 0.0) {
			gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);
		}
	}
	
	if (lum < 0.50) {
		if (mod(gl_FragCoord.x + gl_FragCoord.y - 5.0, 10.0) == 0.0) {
			gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);
		}
	}
	
	if (lum < 0.3) {
		if (mod(gl_FragCoord.x - gl_FragCoord.y - 5.0, 10.0) == 0.0) {
			gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);
		}
	}
}