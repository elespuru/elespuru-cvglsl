/*
 *
 *  Peter R. Elespuru
 *
 *  modified from:
 *  http://cpansearch.perl.org/src/CORION/App-VideoMixer-0.02/filters/
 */
	 
uniform sampler2D img;
varying vec2 texCoord;

void main(void) {
	float frequency = 83.0;
	float offset = 2.0;
	
    float global_pos = (texCoord.y + offset) * frequency;
    float wave_pos = cos((fract( global_pos ) - 0.5)*3.14);
    vec4 pel = texture2D( img, texCoord );

    gl_FragColor = mix(vec4(0,0,0,0), pel, wave_pos);
}