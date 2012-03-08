/*
 *
 *  Peter R. Elespuru
 *	modified from this idea:
 *	http://learningwebgl.com/blog/?p=2858
 *
 */
	
uniform sampler2D img;

vec4 PostFX(sampler2D tex, vec2 uv) {
	
	float stitching_size = 6.0;
	int invert = 0;
	float dim = 600.0;
	
	vec4 c = vec4(0.0);
	float size = stitching_size;
	vec2 cPos = uv * vec2(dim, dim);
	vec2 tlPos = floor(cPos / vec2(size, size));
	tlPos *= size;
	int remX = int(mod(cPos.x, size));
	int remY = int(mod(cPos.y, size));
	if (remX == 0 && remY == 0)
	tlPos = cPos;
	vec2 blPos = tlPos;
	blPos.y += (size - 1.0);
	
	if ((remX == remY) ||
	(((int(cPos.x) - int(blPos.x)) == (int(blPos.y) - int(cPos.y)))))
	{
		if (invert == 1)
		c = vec4(0.2, 0.15, 0.05, 1.0);
		else
		c = texture2D(tex, tlPos * vec2(1.0/dim, 1.0/dim)) * 1.4;
	}
	else
	{
		if (invert == 1)
		c = texture2D(tex, tlPos * vec2(1.0/dim, 1.0/dim)) * 1.4;
		else
		c = vec4(0.0, 0.0, 0.0, 1.0);
	}
	return c;
}

void main (void) {
	vec2 uv = gl_TexCoord[0].st;
	gl_FragColor = PostFX(img, uv);

	return;  
	if (uv.y > 0.5)
	{
		gl_FragColor = PostFX(img, uv);
	}
	else
	{
		uv.y += 0.5;
		vec4 c1 = texture2D(img, uv);
		gl_FragColor = c1;
	}
}

