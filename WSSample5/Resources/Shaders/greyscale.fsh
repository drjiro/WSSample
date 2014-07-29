//
// Gray scale
//

// Global variable
uniform sampler2D u_texture0;

// Variables from a vertex shader
varying vec4 v_color;
varying vec2 v_texCoord;

void main(void)
{
	vec4 c = texture2D(u_texture0, v_texCoord);
	gl_FragColor.xyz = vec3(0.2126*c.r + 0.7152*c.g + 0.0722*c.b);
	gl_FragColor.w = c.w;
}