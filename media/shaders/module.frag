#version 400

uniform vec3 vEyePosition;
uniform vec3 vEyeDirection;
uniform vec4 vLightPosition;

in vec4 color;
in vec3 normal;
in vec4 position;

void main() {

	vec3 lightDirection = normalize(vec3(vLightPosition) - vec3(position));

	
	float diffuse = max(0.0, dot(normal, lightDirection));


	vec3 final = diffuse * vec3(color);
	gl_FragColor = vec4(final, 1.0);
}