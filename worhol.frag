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
	float steps = 2.0;
	float dotsize = 1.0 / steps ;
	float half_step = dotsize / 2.0;

    vec2 img2    = texCoord * steps;
    vec4 pel    = texture2D( img, img2 );
    
    vec4 tint;
    
    // Magenta Blue
    // Cyan    Yellow
    int ofs = int(texCoord.x*steps) + int(texCoord.y*steps)*2;
    if    (0 == ofs) {
        tint = vec4(1.0,1.0,0.0,0.0);
    } else if (1 == ofs) {
        tint = vec4(0.0,0.0,1.0,0.0);
    } else if (2 == ofs) {
        tint = vec4(1.0,0.0,1.0,0.0);
    } else { // (3 == ofs)
        tint = vec4(0.0,1.0,1.0,0.0);
    };
    
    float gray  = dot(pel.rgb, vec3(0.3, 0.59, 0.11));
    gl_FragColor = mix( pel, tint, gray );
}
