#version 130

in vec4 vPosition;
in vec3 vNormal;
in vec4 vTexCoord;
in vec4 vColor;

out vec4 color;


void main() {
	gl_Position = gl_ModelViewProjectionMatrix * vPosition;
	color = vColor;

}