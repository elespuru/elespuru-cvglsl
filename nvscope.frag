/*
 *
 *  Peter R. Elespuru
 *  
 *  modified from:
 *  http://www.geeks3d.com/20091009/shader-library-night-vision-post-processing-filter-glsl/ 
 */
uniform sampler2D img;

void main ()
{
	float elapsedTime = 10.0;
	float luminanceThreshold = 0.2;
	float colorAmplification = 4.0;
	float effectCoverage = 1.0;
	
	vec4 finalColor;
	if (gl_TexCoord[0].x < effectCoverage) {
		vec2 uv;
		uv.x = 0.4*sin(elapsedTime*50.0);
		uv.y = 0.4*cos(elapsedTime*50.0);
	
		vec3 c = texture2D(img, gl_TexCoord[0].st).rgb;

		float lum = dot(vec3(0.30, 0.59, 0.11), c);
		
		if (lum < luminanceThreshold) {
			c *= colorAmplification; 
		}

		vec3 visionColor = vec3(0.1, 0.95, 0.2);
		finalColor.rgb = c * visionColor;
	} else {
		
		finalColor = texture2D(img,	gl_TexCoord[0].st);
	}
	
	gl_FragColor.rgb = finalColor.rgb;
	gl_FragColor.a = 1.0;
}
