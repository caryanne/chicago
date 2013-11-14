#version 400

uniform sampler2D sTexture;

in vec2 UV;
in vec3 normal;


void main() {

	vec4 final = texture( sTexture, UV);
	gl_FragColor = final;


}