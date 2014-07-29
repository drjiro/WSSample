//
// Lambert
//

// Attrubute variable
attribute vec4 a_position;
attribute vec3 a_normal;
attribute vec4 a_color;
attribute vec2 a_texCoord;

// Global variables
uniform mat4 u_worldMatrix;
uniform mat4 u_viewMatrix;
uniform mat4 u_projectionMatrix;
uniform mat4 u_normalMatrix;

uniform vec3 u_lightDirection0;

// Varying variables
varying vec2 v_texCoord;
varying vec4 v_diffuseColor;

const vec3 c_diffuse = vec3(0.7,0.7,0.7);
const vec3 c_ambient = vec3(0.2, 0.2, 0.2);

void main()
{
	// Set the transformed position
	gl_Position = u_projectionMatrix * u_viewMatrix * u_worldMatrix * a_position;
    
	// Pass through parameters
	v_texCoord = a_texCoord;
    
	// Normal vector in view space
	vec3 N = normalize((u_normalMatrix * vec4(a_normal, 1)).xyz);
	vec3 L = normalize(-u_lightDirection0);
    
	// Diffuse color
	float df = max(0.0, dot(N, L));
	
	vec3 color = df * c_diffuse + c_ambient;
	
	v_diffuseColor = vec4(color, 1);
}