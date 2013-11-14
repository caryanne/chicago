#version 400

uniform mat4 mModelView;
uniform mat4 mModelViewProj;

uniform vec3 vEyePosition;
uniform vec3 vEyeDirection;
uniform vec4 vLightPosition;

in vec3 vPosition;
in vec3 vNormal;
in vec3 vTangent;
in vec2 vTexCoord;

out vec2 UV;
out vec4 color;
out vec3 normal;
out vec4 position;

void main() {

	UV = vTexCoord;
	color = vec4(0.2, 0.2, 0.2, 1.0);
	normal = vNormal;
	gl_Position =  mModelViewProj * vec4(vPosition, 1.0);
	
}