varying vec2 uv;
varying vec2 pos;

uniform float u_pen_size;
uniform vec4 u_pen_color;
uniform vec2 u_mouse_pos;
uniform sampler2D u_texture;

void main() {
	vec4 color = texture2D(u_texture, uv);
	
	{
		vec2 d = pos - u_mouse_pos;
		if (dot(d, d) < u_pen_size * u_pen_size)
			color = mix(color, u_pen_color, 0.5);
	}
	
	gl_FragColor = color;
}
