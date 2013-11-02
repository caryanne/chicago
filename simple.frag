#version 130

in vec4 color;
in float intensity;

void main() {
	vec4 diffuse = vec4(206/255.0,53/255.0,39/255.0,1.0);
	gl_FragColor = diffuse * intensity;
	gl_FragColor.a = 1.0;



}