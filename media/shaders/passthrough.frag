#version 400

in vec4 color;
in float diffuse;


void main() {

	vec3 final = diffuse * vec3(color);
	gl_FragColor = vec4(final, 1.0);
}