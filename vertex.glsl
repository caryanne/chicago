#version 130

in vec4 s_vpos;
in vec4 s_vcolor;

out vec4 color;

void main() {
	gl_FrontColor = s_vcolor;
	gl_Position = s_vpos;

}