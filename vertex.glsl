#version 130

in vec4 vpos;
in vec4 vcolor;
in vec4 vnormal;

out vec4 color;

void main() {
	gl_Position = vpos * gl_ModelViewProjectionMatrix;
	color = vcolor;
}