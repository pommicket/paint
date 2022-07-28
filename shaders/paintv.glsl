attribute vec2 v_pos;
attribute vec2 v_uv;

uniform vec2 u_size;

varying vec2 uv;
varying vec2 pos;

void main() {
	gl_Position = vec4(v_pos, 0.0, 1.0);
	uv = v_uv;
	pos = (v_pos * vec2(0.5, -0.5) + 0.5) * u_size;
}
