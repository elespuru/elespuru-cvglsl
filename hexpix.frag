/*
 *
 *  Peter R. Elespuru
 *  
 *  Derived from:
 *  http://coding-experiments.blogspot.com/2010/06/pixelation.html 
 */

uniform sampler2D img;

vec2 hexCoord(ivec2 hexIndex) {
	
	float H = 0.01;
	float S = ((3./2.) * H/sqrt(3.));
		
	int i = int(hexIndex.x);
	int j = int(hexIndex.y);
	vec2 r = vec2(0,0);
	r.x = float(i) * S;
	r.y = float(j) * H + float(i%2) * H/2.;
	return r;
	
}

ivec2 hexIndex(vec2 coord) {
	
	float H = 0.01;
	float S = ((3./2.) * H/sqrt(3.));
	
	ivec2 r;
	float x = coord.x;
	float y = coord.y;
	int it = int(floor(x/S));
	float yts = y - float(it%2) * H/2.;
	int jt = int(floor((1./H) * yts));
	float xt = x - float(it) * S;
	float yt = yts - float(jt) * H;
	int deltaj = (yt > H/2.)? 1:0;
	float fcond = S * (2./3.) * abs(0.5 - yt/H);

	if (xt > fcond) {
		r.x = it;
		r.y = jt;
		
	} else {
		r.x = it - 1;
		r.y = jt - (r.x%2) + deltaj;
		
	}

	return r;
}

void main(void)
{
	vec2 xy = gl_TexCoord[0].xy;
	ivec2 hexIx = hexIndex(xy);
	vec2 hexXy = hexCoord(hexIx);
	vec4 fcol = texture2D(img, hexXy);
	gl_FragColor = fcol;
}