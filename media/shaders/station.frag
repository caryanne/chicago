#version 130

in vec4 color;
in float intensity;

void main() {
	vec4 diffuse = vec4(1,.5,0,1.0);
	gl_FragColor = diffuse * intensity * 2;
	gl_FragColor.a = 1;



}