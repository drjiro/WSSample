//
// Mosaic 2-D texture
//

#ifdef GL_ES
precision mediump float;
#endif

// Variables from a vertex shader
varying vec4 v_color;
varying vec2 v_texCoord;

// Global variable
uniform sampler2D u_texture0;

void main() {
	float grids = 2.0;
    vec2 uv = floor(v_texCoord * grids + 0.5) / grids;
	// Texture color only
    gl_FragColor = texture2D(u_texture0, uv);
//    gl_FragColor = v_color * texture2D(u_texture0, v_texCoord);
//	gl_FragColor = vec4(1.0, 0.0, 0.0, 0.5);
}

