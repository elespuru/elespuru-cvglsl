// Copy verbatim

uniform sampler2D img;

void main()
{
   gl_FragColor = texture2D(img,gl_TexCoord[0].st);
}
