//
// Lambert
//

// Global variables
uniform sampler2D u_texture0;

// Varying variables
varying vec2 v_texCoord;
varying vec4 v_diffuseColor;

void main(void) {
	gl_FragColor = texture2D(u_texture0, v_texCoord) * v_diffuseColor;
}
