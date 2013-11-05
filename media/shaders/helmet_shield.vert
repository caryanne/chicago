#version 400

uniform mat4 mModelView;
uniform mat4 mModelViewProj;
uniform mat3 mNormalMatrix;
uniform vec3 vEye;

in vec3 vPosition;
in vec3 vNormal;
in vec2 vTexCoord;

out vec2 UV;
out vec4 color;
out float intensity;

void main() {

	

	gl_Position = mModelViewProj * vec4(vPosition,1.0);
}