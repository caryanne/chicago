#version 400
uniform sampler2D sTexture;
uniform vec3 vEyeDirection;
uniform vec4 vLightPosition;

in vec2 UV;
in float diffuse;


void main() {
	vec4 final = diffuse * texture( sTexture, UV);
	final.a = 1.0;
	gl_FragColor = final;
}