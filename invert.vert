/*
 *
 *  Peter R. Elespuru
 *  
 */
varying vec2 texCoord;

void main (void) {
    gl_TexCoord[0] = gl_MultiTexCoord0;
    gl_Position = ftransform();
	texCoord = vec2(gl_TexCoord[0].xy);
	
}
