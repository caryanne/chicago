#version 400
uniform sampler2D sTexture;
uniform sampler2D sNormal;

uniform vec3 vEyePosition;
uniform vec3 vEyeDirection;
uniform vec4 vLightPosition;

in vec2 UV;
in vec4 color;
in vec3 normal;
in vec4 position;

void main() {




	vec3 lightDirection = normalize(vec3(vLightPosition) - vec3(position));
	float diffuse = max(0.0, dot(normal, lightDirection));


	vec4 final = diffuse * texture( sTexture, UV);
	final.a = 1.0;
	gl_FragColor = final;
}