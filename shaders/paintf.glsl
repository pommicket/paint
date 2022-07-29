varying vec2 uv;
varying vec2 pos;

uniform float u_pen_size;
uniform vec2 u_prev_mouse_pos;
uniform vec2 u_mouse_pos;
uniform sampler2D u_texture;
uniform vec4 u_pen_color;
uniform int u_lmouse_down;

void main() {
	vec4 color = texture2D(u_texture, uv);
	float d = dot(pos - u_mouse_pos, pos - u_mouse_pos);
	float intensity = (u_pen_size * u_pen_size - d) / (u_pen_size * u_pen_size);
	if (u_lmouse_down != 0 && intensity > 0.0) {
		intensity *= intensity;
		color = sqrt(mix(color * color, u_pen_color * u_pen_color, intensity * intensity));
	}
	
	gl_FragColor = color;
}
