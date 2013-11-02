#version 400

uniform mat4 mModelView;
uniform mat4 mModelViewProj;
uniform mat3 mNormalMatrix;

uniform vec3 vEye;

in vec3 vPosition;
in vec3 vNormal;
in vec4 vTexCoord;

out vec4 color;
out float intensity;

void main() {

	vec3 tnorm = normalize(mNormalMatrix * vNormal);

	vec4 eyeCoords = mModelView * vec4(vPosition,1.0);

	vec4 lightpos = mModelView * vec4(10.0,10.0,10.0,1.0f);

	vec3 s = normalize(	vec3(		lightpos - eyeCoords	)	 );
	
	intensity =max( dot( s, tnorm ), 0.0 );

	gl_Position = mModelViewProj * vec4(vPosition,1.0);
}