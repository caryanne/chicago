#version 400

uniform mat4 mModelView;
uniform mat4 mModelViewProj;
uniform mat3 mNormalMatrix;

uniform vec4 vLightPosition;

in vec3 vPosition;
in vec3 vNormal;
in vec2 vTexCoord;
in vec3 vTangent;

out vec3 light;
out vec3 eye;
out vec2 UV;
out float distance; 
void main() {
	UV = vTexCoord;
	gl_Position = mModelViewProj * vec4(vPosition, 1.0);
	vec3 normal = normalize(mNormalMatrix * vNormal);
	vec3 tangent = normalize(mNormalMatrix * vTangent);
	vec3 binormal = cross(normal, tangent);

	vec3 position = (mModelView * vec4(vPosition, 1.0)).xyz;

	vec3 lightDirection = vec3(vLightPosition) - vec3(position);
	distance = length(lightDirection);
	lightDirection = lightDirection / distance;

	vec3 v;
	v.x = dot(lightDirection, tangent);
	v.y = dot(lightDirection, binormal);
	v.z = dot(lightDirection, normal);
	light = normalize(v);

	v.x = dot(position, tangent);
	v.y = dot(position, binormal);
	v.z = dot(position, normal);
	eye = normalize(v);
	
}
