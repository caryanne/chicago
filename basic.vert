#version 400

in vec3 vPosition;
in vec3 vNormal;
//in vec4 vTexCoord;
//in vec4 vColor;

out vec4 color;


void main() {
	gl_Normal = vNormal;
	gl_Position = gl_ModelViewProjectionMatrix * vec4(vPosition,1.0);
	color = vec4(vNormal,1.0);;//vec4(0.0,0.0,0.8,1.0);

}