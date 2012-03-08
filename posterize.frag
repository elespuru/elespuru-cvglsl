/*
 *
 *  Peter R. Elespuru
 *
 *  common approach to posterization
 */
	 
uniform sampler2D img;

void main() {
	
	float gamma = 0.6;
	float numColors = 8.0;
	
	vec3 c = texture2D(img, gl_TexCoord[0].xy).rgb;
	c = pow(c, vec3(gamma, gamma, gamma));
	c = c * numColors;
	c = floor(c);
	c = c / numColors;
	c = pow(c, vec3(1.0/gamma));
	gl_FragColor = vec4(c, 1.0);
}

