/*
 *
 *  Peter R. Elespuru
 *  
 *  modified from this idea:
 *  http://www.geeks3d.com/20091112/shader-library-dream-vision-post-processing-filter-glsl/
 */

uniform sampler2D img;

void main (void) {
  vec2 uv = gl_TexCoord[0].xy;
  vec4 c = texture2D(img, uv);

  c += texture2D(img, uv+0.001);
  c += texture2D(img, uv+0.003);
  c += texture2D(img, uv+0.005);
  c += texture2D(img, uv+0.007);
  c += texture2D(img, uv+0.009);
  c += texture2D(img, uv+0.011);

  c += texture2D(img, uv-0.001);
  c += texture2D(img, uv-0.003);
  c += texture2D(img, uv-0.005);
  c += texture2D(img, uv-0.007);
  c += texture2D(img, uv-0.009);
  c += texture2D(img, uv-0.011);

  c.rgb = vec3((c.r+c.g+c.b)/3.0);
  c = c / 9.5;
  gl_FragColor = c;
  
}