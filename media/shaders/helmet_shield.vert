#version 400

uniform mat4 mModelView;
uniform mat4 mModelViewProj;
uniform mat3 mNormalMatrix;
uniform vec3 vEyePosition;
uniform vec3 vEyeDirection;
uniform vec4 vLightPosition;

in vec3 vPosition;
in vec3 vNormal;
in vec2 vTexCoord;

out vec2 UV;
out vec4 color;
out vec3 normal;
out vec4 position;

void main() {

	UV = vTexCoord;
	color = vec4(0.4, 0.4, 0.9, 1.0);
	normal = normalize(mNormalMatrix * vNormal);
	position = mModelView * vec4(vPosition, 1.0);
	gl_Position =  mModelViewProj * vec4(vPosition, 1.0);
	
}