#version 400
uniform sampler2D sTexture;
uniform sampler2D sNormal;

in vec3 light;
in vec3 eye;
in vec3 halfv;
in vec2 UV;
in float distance;

void main() {

	vec3 normal = normalize(2.0 * texture2D(sNormal, UV).rgb - 1.0);
	
	float diffuse = max(0.0, dot(normal, light)) * pow(8.0 / distance, 2);
	vec4 final = diffuse * texture2D(sTexture, UV);
	final.a = 1.0;
	gl_FragColor = final;

	/*vec4 diffuseMat = 0.0;
	vec4 diffuseLight = 0.0;

	vec4 specularMat;
	vec4 specularLight;
	float shininess;

	vec4 ambient = vec4(0.1);

	if(lamber > 0.0) {
		diffuseMat = texture2D(sTexture, UV);
		diffuseLight = vec4(1.0);

		specularMat = vec4(1.0);
		specularLight = vec4(1.0);
		shininess = pow(max(0.0, dot(halfv, normal)), 2.0);


		gl_FragColor = diffuseMat * diffuseLight * lamber;
		gl_FragColor += specularMat * specularLight * shininess;
	}
	gl_FragColor.a = 1.0;
	gl_FragColor += ambient;*/
}