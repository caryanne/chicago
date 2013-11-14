#version 400
uniform sampler2D sTexture;
uniform sampler2D sNormal;
uniform vec3 vEyePosition;
uniform vec3 vEyeDirection;
uniform vec4 vLightPosition;
uniform mat3 mNormalMatrix;

in vec2 UV;
in vec4 color;
in vec3 normal;


void main() {
	vec3 bump = normalize(texture(sNormal, UV).xyz * 2.0 - 1.0);
	vec3 bumpnormal = normalize(mNormalMatrix * normalize(normal + bump));

	vec3 lightDirection = vec3(vLightPosition) - vec3(position);
	float lightDistance = length(lightDirection);
	lightDirection = lightDirection / lightDistance;
	
	float diffuse = max(0.0, dot(bumpnormal, lightDirection));

	vec4 tex = texture( sTexture, UV);
	vec4 final = diffuse * tex;
	final.a = 1.0;
	gl_FragColor = final;
}