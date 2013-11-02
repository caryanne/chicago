#version 130

in vec4 vPosition;
in vec4 vNormal;
in vec4 vTexCoord;

out vec4 color;

void main() {
	gl_Position = vPosition * gl_ModelViewProjectionMatrix;
	color = vec4(1.0,0.0,0.0,1.0);//vNormal;
}