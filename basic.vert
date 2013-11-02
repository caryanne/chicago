#version 400

uniform mat4 mModelView;
uniform mat4 mModelViewProj;
uniform mat3 mNormalMatrix;

uniform vec3 vEye;

uniform float time;

in vec3 vPosition;
in vec3 vNormal;
in vec2 vTexCoord;

out vec2 UV;
out vec4 color;
out float intensity;

void main() {

	vec3 tnorm = normalize(mNormalMatrix * vNormal);

	vec4 eyeCoords = mModelView* vec4(vPosition,1.0);

	vec4 lightpos = mModelView*vec4(10 * sin(time),0.0,10.0 * cos(time),1.0f);

	vec3 s = normalize(vec3(lightpos - eyeCoords));
	
	intensity =max( dot( s, tnorm ), 0.0 );
	UV = vTexCoord;
	gl_Position = mModelViewProj * vec4(vPosition,1.0);
}