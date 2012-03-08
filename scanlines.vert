/*
 *
 *  Peter R. Elespuru
 *  
 *  modified from:
 *  http://cpansearch.perl.org/src/CORION/App-VideoMixer-0.02/filters/
 */

varying vec2  texCoord;

void main(void) {
//   gl_Position = vec4( gl_Vertex.xy, 0.0, 1.0 );
//   gl_Position = sign( gl_Position );
   gl_Position    = ftransform();
   gl_TexCoord[0] = gl_MultiTexCoord0;
   
   //texCoord = (vec2( gl_Position.x, gl_Position.y ) + vec2( 1.0 ) ) / vec2( 2.0 );
	texCoord = vec2(gl_TexCoord[0].xy);
}
