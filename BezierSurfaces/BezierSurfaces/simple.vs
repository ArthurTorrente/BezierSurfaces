attribute vec3 a_position;

uniform vec3 u_color;
uniform mat4 u_proj;
uniform mat4 u_view;
uniform mat4 u_world;

varying vec4 v_color;
varying vec3 v_normal;

const vec3 c_lightDirection = vec3(1.0, 1.0, 0.0);

void main(void)
{
	gl_Position = u_proj * u_view * u_world * vec4(a_position, 1.0);
	
	vec3 pseudoNormal = normalize(a_position.xyz);
	
	vec3 normal = (u_world * vec4(pseudoNormal, 1.0)).xyz;
	v_normal = normal;
	
	vec3 L = normalize(c_lightDirection);
	float NdotL = max(dot(v_normal, L), 0.0);
	v_color = vec4(u_color, 1.0) * NdotL;
}