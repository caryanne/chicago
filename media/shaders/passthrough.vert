#version 400

uniform mat4 mModelView;
uniform mat4 mModelViewProj;
uniform mat3 mNormalMatrix;
uniform vec4 vEyePosition;

in vec3 vPosition;
in vec3 vNormal;

out vec4 color;
out float diffuse;

void main() {
	vec4 position = mModelView * vec4(vPosition, 1.0);
	color = vec4(1.0, 1.0, 1.0, 1.0);
	vec3 normal = normalize(mNormalMatrix * vNormal);
	vec3 lightDirection = normalize(vec3(vEyePosition) - vec3(position));
	diffuse = max(0.0, dot(normal, lightDirection));
	gl_Position = mModelViewProj * vec4(vPosition, 1.0);
}