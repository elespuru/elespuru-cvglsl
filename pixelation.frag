/*
 *
 *  Peter R. Elespuru
 *
 *  modified from:
 *  http://www.geeks3d.com/20101029/shader-library-pixelation-post-processing-effect-glsl/
 */
	 
uniform sampler2D tex;

void main() {
	
 float dx = 5.*(1./600.);
 float dy = 3.*(1./600.);
 
 vec2 coord = vec2(dx*floor(gl_TexCoord[0].x/dx),
                   dy*floor(gl_TexCoord[0].y/dy));
 
 gl_FragColor = texture2D(tex, coord);

}
 