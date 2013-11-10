#version 400

uniform vec3 vEyePosition;
uniform vec3 vEyeDirection;
uniform vec4 vLightPosition;

in vec4 color;
in vec3 normal;
in vec4 position;

void main() {

	vec3 lightDirection = vec3(vLightPosition) - vec3(position);
	float lightDistance = length(lightDirection);
	lightDirection = lightDirection / lightDistance;
	
	float diffuse = max(0.3, dot(-1 * normal, lightDirection));
	vec3 final = diffuse * vec3(color);
	if(diffuse > 0.99)
		gl_FragColor = vec4(1, 1, 1, 0.5);
	else
		gl_FragColor = vec4(final, 0.2);
}