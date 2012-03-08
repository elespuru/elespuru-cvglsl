// Dilation
// maximum of 3x3 kernel

uniform float dX;
uniform float dY;
uniform sampler2D img;

vec4 sample(float dx,float dy)
{
   return texture2D(img,gl_TexCoord[0].st+vec2(dx,dy));
}

void main()
{
   vec4 color = sample(0.0,0.0);
   color = max(color , sample(-dX,+dY));
   color = max(color , sample(0.0,+dY));
   color = max(color , sample(+dX,+dY));
   color = max(color , sample(-dX,0.0));
   color = max(color , sample(+dX,0.0));
   color = max(color , sample(-dX,-dY));
   color = max(color , sample(0.0,-dY));
   color = max(color , sample(+dX,-dY));
   gl_FragColor = color;
}
