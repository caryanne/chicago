#version 400

uniform mat4 mModelView;
uniform mat4 mModelViewProj;
uniform mat3 mNormalMatrix;

in vec3 vPosition;
in vec3 vNormal;
in vec2 vTexCoord;

out vec2 UV;
out vec3 normal;

void main() {
	UV = vTexCoord;
	normal = normalize(mNormalMatrix * vNormal);
	gl_Position =  mModelViewProj * vec4(vPosition, 1.0);
	
}