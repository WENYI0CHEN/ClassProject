// glsl
varying vec4 v_color; // input
varying vec2 v_uv;

uniform sampler2D u_texture;
uniform vec3 u_color;

void main()
{
	gl_FragColor = vec4( u_color.rgb, 1.0 );
}
