#version 130

uniform sampler2D sTexture;

in vec4 color;
in float intensity;
in vec2 UV;

void main() {

	vec4 inter;
	vec4 diffuse = texture( sTexture, UV);

	inter = diffuse * intensity;

	inter.a=1.0;
	gl_FragColor = inter;


}