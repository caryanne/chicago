#version 400

uniform sampler2D sTexture;

in vec2 UV;
in vec4 color;
in vec3 normal;
in vec4 position;

void main() {


	vec4 final = texture( sTexture, UV);
	gl_FragColor = final;


}