#version 130

in vec4 color;
in float intensity;

void main() {

if (intensity > 0.95)
		gl_FragColor = vec4(1.0,0.5,0.5,1.0);
	else if (intensity > 0.5)
		gl_FragColor = vec4(0.6,0.3,0.3,1.0);
	else if (intensity > 0.25)
		gl_FragColor = vec4(0.4,0.2,0.2,1.0);
	else
		gl_FragColor = vec4(0.2,0.1,0.1,1.0);


}