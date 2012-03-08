/*
 *
 *  Peter R. Elespuru
 *  
 *  derived from: http://www.forceflow.be/thesis/thesis-code/
 */

// texture
uniform sampler2D img;

float intensity(in vec4 color) {
	return sqrt((color.x*color.x)+(color.y*color.y)+(color.z*color.z));
}

vec3 radial_edge_detection(in float step, in vec2 center) {
	
	int radius = 5;
	int renderwidth = 600;

	// let's learn more about our center pixel
	float center_intensity = intensity(texture2D(img, center));
	
	// counters we need
	int darker_count = 0;
	float max_intensity = center_intensity;
	
	// let's look at our neighbouring points
	for(int i = -radius; i <= radius; i++) {
		for(int j = -radius; j<= radius; j++) {
			
			vec2 current_location = center + vec2(float(i)*step, float(j)*step);
			float current_intensity = intensity(texture2D(img,current_location));
			
			if(current_intensity < center_intensity) {
				darker_count++;
			}
			
			if(current_intensity > max_intensity) {
				max_intensity = current_intensity;
			}
		}
	}
	
	// do we have a valley pixel?
	if((max_intensity - center_intensity) > 0.01*float(radius)) {
		if(darker_count/(radius*radius) < (1-(1/radius))) {
			return vec3(0.0,0.0,0.0); // yep, it's a valley pixel.
			
		}
	}
	
	return vec3(1.0,1.0,1.0); // no, it's not.

}

void main(void) {
	
	int radius = 5;
	int renderwidth = 600;
	
	float step = 1.0/float(renderwidth);
	vec2 center_color = gl_TexCoord[0].st;
	gl_FragColor.xyz = radial_edge_detection(step,center_color);
    gl_FragColor.a = 0.0;
	
}



