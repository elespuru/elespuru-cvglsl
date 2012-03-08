// Laplacian edge detection
//   -1 -1 -1
//   -1  8 -1
//   -1 -1 -1

uniform float dX;
uniform float dY;
uniform sampler2D img;

vec4 sample(float dx,float dy)
{
   return texture2D(img,gl_TexCoord[0].st+vec2(dx,dy));
}

void main()
{
   gl_FragColor = -sample(-dX,+dY) -     sample(0.0,+dY) - sample(+dX,+dY)
                  -sample(-dX,0.0) + 8.0*sample(0.0,0.0) - sample(+dX,0.0)
                  -sample(-dX,-dY) -     sample(0.0,-dY) - sample(+dX,-dY);
}
