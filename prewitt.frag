// Prewitt edge detection
//    -1 -1 -1       1 0 -1
// H = 0  0  0   V = 1 0 -1
//     1  1  1       1 0 -1
//
// result = sqrt(H^2 + V^2)

uniform float dX;
uniform float dY;
uniform sampler2D img;

vec4 sample(float dx,float dy)
{
   return texture2D(img,gl_TexCoord[0].st+vec2(dx,dy));
}

void main()
{
   vec4 H = -sample(-dX,+dY) - sample(0.0,+dY) - sample(+dX,+dY)
            +sample(-dX,-dY) + sample(0.0,-dY) + sample(+dX,-dY);

   vec4 V = sample(-dX,+dY)  - sample(+dX,+dY)
          + sample(-dX,0.0)  - sample(+dX,0.0)
          + sample(-dX,-dY)  - sample(+dX,-dY);

   gl_FragColor = sqrt(H*H+V*V);
}
