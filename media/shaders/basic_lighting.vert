#version 400

uniform mat4 mModelView;
uniform mat4 mModelViewProj;
uniform mat3 mNormalMatrix;
uniform vec4 vLightPosition;

in vec3 vPosition;
in vec3 vNormal;

out vec4 color;
out float diffuse;

void main() {
	color = vec4(0.9, 0.9, 0.9, 1.0);
	vec4 position = mModelView * vec4(vPosition, 1.0);
	vec3 normal = normalize(mNormalMatrix * vNormal);
	vec3 lightDirection = vec3(vLightPosition) - vec3(position);
	float lightDistance = length(lightDirection);
	lightDirection = lightDirection / lightDistance;

	diffuse = max(0.0, dot(normal, lightDirection)) * pow(10.0 / lightDistance, 2);
	gl_Position = mModelViewProj * vec4(vPosition, 1.0);
}