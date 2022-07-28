varying vec2 uv;
varying vec2 pos;

uniform float u_pen_size;
uniform vec2 u_prev_mouse_pos;
uniform vec2 u_mouse_pos;
uniform sampler2D u_texture;
uniform vec4 u_pen_color;
uniform int u_lmouse_down;

float sqdist_to_line_segment(vec2 p, vec2 a, vec2 b) {
	vec2 ba = b - a;
	vec2 pa = p - a;
	float h = clamp(dot(pa, ba) / dot(ba, ba), 0., 1.);
	vec2 d = pa - h * ba;
	return dot(d, d);
}

void main() {
	vec4 color = texture2D(u_texture, uv);
	
	{
		float d = sqdist_to_line_segment(pos, u_prev_mouse_pos, u_mouse_pos);
		if (u_lmouse_down != 0 && d < u_pen_size * u_pen_size)
			color = u_pen_color;
	}
	
	gl_FragColor = color;
}
