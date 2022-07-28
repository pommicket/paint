/*
conventions:
- vecn = n-dimensional float vector
- mat4 = 4x4 matrix, column-major
- destination always comes first
*/
#define PI 3.14159265358979
#define PIf 3.14159265358979f
#define FLOAT_PRINTF "%.3g"

#ifdef DEBUG
#if _WIN32
// i fucking hate windows
#define debug_printf(...) do {\
	char buf[1024];\
	snprintf(buf, sizeof buf-1, __VA_ARGS__);\
	OutputDebugStringA(buf);\
	}while(0)
#else
#define debug_printf printf
#endif

#define debug_printf_once(...) do {\
	static bool printed = false;\
	if (!printed) { debug_printf(__VA_ARGS__); printed = true; }\
	} while(0)
#else
#define debug_printf(...)
#define debug_printf_once(...)
#endif

float modulusf(float a, float b) {
	float m = fmodf(a, b);
	if (m < 0) m += b;
	return m;
}

#if 1 // faster rand
#define rand rand_fast
#define srand srand_fast
#undef RAND_MAX
#define RAND_MAX 2147483647
#endif

static uint32_t randfast_x=123456789, randfast_y=362436069, randfast_z=521288629;
int rand_fast(void) {
	uint32_t t;
	uint32_t x = randfast_x;
	x ^= x << 16;
	x ^= x >> 5;
	x ^= x << 1;
	
	t = x;
	randfast_x = randfast_y;
	randfast_y = randfast_z;
	randfast_z = t ^ randfast_x ^ randfast_y;
	return (int)(randfast_z & RAND_MAX);
}

void srand_fast(unsigned seed) {
	randfast_x=123456789, randfast_y=362436069, randfast_z=521288629;
	randfast_x ^= seed;
}


float minf(float a, float b) {
	return a <= b ? a : b;
}

float maxf(float a, float b) {
	return a >= b ? a : b;
}

int mini(int a, int b) {
	return a <= b ? a : b;
}

int maxi(int a, int b) {
	return a >= b ? a : b;
}

uint64_t maxu64(uint64_t a, uint64_t b) {
	return a >= b ? a : b;
}

int64_t maxi64(int64_t a, int64_t b) {
	return a >= b ? a : b;
}

int clampi(int x, int a, int b) {
	if (x <= a) return a;
	if (x >= b) return b;
	return x;
}

float clampf(float x, float a, float b) {
	if (x <= a) return a;
	if (x >= b) return b;
	return x;
}

void swapf(float *a, float *b) {
	float temp = *a;
	*a = *b;
	*b = temp;
}

void swapi(int *a, int *b) {
	int temp = *a;
	*a = *b;
	*b = temp;
}


// random float in [0,1)
static float randf(void) {
	float f = (float)rand() * (1/(1+(float)RAND_MAX));
	if (f == 1) // e.g. (2^31 - 1) / (2^31) rounds to 1
		return 0.99999994f;
	return f;
}

// random int in [0, 2^31)
static int randi32(void) {
#if RAND_MAX == 2147483647
	return rand();
#elif RAND_MAX == 32767
	return rand() + rand() << 15 + (rand() & 3) << 30;
#else
	#error "unrecognized RAND_MAX"
#endif
}

static float rand_normal(void) {
	// https://en.wikipedia.org/wiki/Normal_distribution#Generating_values_from_normal_distribution
	float U, V;
	do {
		U = randf(), V = randf();
	} while (U == 0 || V == 0);
	return sqrtf(-2 * logf(U)) * cosf(2 * (float)PI * V);
}

static void vec2_set(float out[2], float x, float y) {
	out[0] = x;
	out[1] = y;
}


static void vec2_scale(float out[2], const float in[2], float by) {
	float x = in[0], y = in[1];
	out[0] = x * by;
	out[1] = y * by;
}

static void vec2_add2f(float out[2], const float in[2], float dx, float dy) {
	float x = in[0], y = in[1];
	out[0] = x + dx;
	out[1] = y + dy;
}

static void vec2_copy(float out[2], const float in[2]) {
	out[0] = in[0];
	out[1] = in[1];
}

static void vec2_add(float out[2], const float a[2], const float b[2]) {
	float ax = a[0], ay = a[1];
	float bx = b[0], by = b[1];
	out[0] = ax + bx;
	out[1] = ay + by;
}

static void vec2_add_scaled(float out[2], const float a[2], const float b[2], float b_scale) {
	float ax = a[0], ay = a[1];
	float bx = b[0], by = b[1];
	out[0] = ax + bx * b_scale;
	out[1] = ay + by * b_scale;
	
}

static void vec2_sub(float out[2], const float a[2], const float b[2]) {
	float ax = a[0], ay = a[1];
	float bx = b[0], by = b[1];
	out[0] = ax - bx;
	out[1] = ay - by;
}

static void vec2_mul(float out[2], const float a[2], const float b[2]) {
	float ax = a[0], ay = a[1];
	float bx = b[0], by = b[1];
	out[0] = ax * bx;
	out[1] = ay * by;
}

static void vec2_div(float out[2], const float a[2], const float b[2]) {
	float ax = a[0], ay = a[1];
	float bx = b[0], by = b[1];
	out[0] = ax / bx;
	out[1] = ay / by;
}

static float vec2_dot(const float a[2], const float b[2]) {
	return a[0] * b[0] + a[1] * b[1];
}

static float vec2_length(const float a[2]) {
	return sqrtf(vec2_dot(a, a));
}

static float vec2_dist_squared(const float a[2], const float b[2]) {
	float d[2];
	vec2_sub(d, a, b);
	return vec2_dot(d, d);
}

static float vec2_dist(const float a[2], const float b[2]) {
	return sqrtf(vec2_dist_squared(a, b));
}

static void vec2_normalize(float out[2], const float a[2]) {
	float l = vec2_length(a);
	if (l != 0) {
		vec2_scale(out, a, 1 / l);
	} else {
		out[0] = 1;
		out[1] = 0;
	}
}

// out := a, scaled so that its length is `scale`.
static void vec2_scale_to(float out[2], const float a[2], float scale) {
	float l = vec2_length(a);
	if (l != 0) {
		vec2_scale(out, a, scale / l);
	} else {
		out[0] = scale;
		out[1] = 0;
	}
}

static void vec2_floor(float out[2], const float in[2]) {
	float x = in[0], y = in[1];
	out[0] = floorf(x);
	out[1] = floorf(y);
}

static void vec2_abs(float out[2], const float in[2]) {
	float x = in[0], y = in[1];
	out[0] = fabsf(x);
	out[1] = fabsf(y);
}

static void vec3_set(float out[3], float x, float y, float z) {
	out[0] = x;
	out[1] = y;
	out[2] = z;
}

static void vec3_scale(float out[3], const float in[3], float by) {
	float x = in[0], y = in[1], z = in[2];
	out[0] = x * by;
	out[1] = y * by;
	out[2] = z * by;
}

static void vec3_add3f(float out[3], const float in[3], float dx, float dy, float dz) {
	float x = in[0], y = in[1], z = in[2];
	out[0] = x + dx;
	out[1] = y + dy;
	out[2] = z + dz;
}

static void vec3_copy(float out[3], const float in[3]) {
	out[0] = in[0];
	out[1] = in[1];
	out[2] = in[2];
}

static void vec3_add(float out[3], const float a[3], const float b[3]) {
	float ax = a[0], ay = a[1], az = a[2];
	float bx = b[0], by = b[1], bz = b[2];
	out[0] = ax + bx;
	out[1] = ay + by;
	out[2] = az + bz;
}

static void vec3_add_scaled(float out[3], const float a[3], const float b[3], float b_scale) {
	float ax = a[0], ay = a[1], az = a[2];
	float bx = b[0], by = b[1], bz = b[2];
	out[0] = ax + bx * b_scale;
	out[1] = ay + by * b_scale;
	out[2] = az + bz * b_scale;
	
}

static void vec3_sub(float out[3], const float a[3], const float b[3]) {
	float ax = a[0], ay = a[1], az = a[2];
	float bx = b[0], by = b[1], bz = b[2];
	out[0] = ax - bx;
	out[1] = ay - by;
	out[2] = az - bz;
}

static void vec3_cross(float out[3], const float a[3], const float b[3]) {
	float ax = a[0], ay = a[1], az = a[2];
	float bx = b[0], by = b[1], bz = b[2];
	out[0] = ay*bz - az*by;
	out[1] = az*bx - ax*bz;
	out[2] = ax*by - ay*bx;
}

static float vec3_dot(const float a[3], const float b[3]) {
	return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

static float vec3_length(const float a[3]) {
	return sqrtf(vec3_dot(a, a));
}

static void vec3_normalize(float out[3], const float a[3]) {
	float l = vec3_length(a);
	if (l != 0) {
		vec3_scale(out, a, 1 / l);
	} else {
		out[0] = 1;
		out[1] = 0;
		out[2] = 0;
	}
}

// out := a, scaled so that its length is `scale`.
static void vec3_scale_to(float out[3], const float a[3], float scale) {
	float l = vec3_length(a);
	if (l != 0) {
		vec3_scale(out, a, scale / l);
	} else {
		out[0] = scale;
		out[1] = 0;
		out[2] = 0;
	}
}

static void vec3_mul(float out[3], const float a[3], const float b[3]) {
	float ax = a[0], ay = a[1], az = a[2];
	float bx = b[0], by = b[1], bz = b[2];
	out[0] = ax * bx;
	out[1] = ay * by;
	out[2] = az * bz;
}

static void vec3_div(float out[3], const float a[3], const float b[3]) {
	float ax = a[0], ay = a[1], az = a[2];
	float bx = b[0], by = b[1], bz = b[2];
	out[0] = ax / bx;
	out[1] = ay / by;
	out[2] = az / bz;
}

static void vec3_abs(float out[3], const float in[3]) {
	float x = in[0], y = in[1], z = in[2];
	out[0] = fabsf(x);
	out[1] = fabsf(y);
	out[2] = fabsf(z);
}

static void vec3_apply_yaw(float out[3], const float in[3], float yaw) {
	float x = in[0], y = in[1], z = in[2];
	float c = cosf(yaw), s = sinf(yaw);
	out[0] = c * x + s * z;
	out[1] = y;
	out[2] = c * z - s * x;
}
static void vec3_apply_pitch(float out[3], const float in[3], float pitch) {
	float x = in[0], y = in[1], z = in[2];
	float c = cosf(pitch), s = sinf(pitch);
	out[0] = x;
	out[1] = c * y - s * z;
	out[2] = s * y + c * z;
}

static void vec4_set(float out[4], float x, float y, float z, float w) {
	out[0] = x;
	out[1] = y;
	out[2] = z;
	out[3] = w;
}

static void vec4_scale(float out[4], const float in[4], float by) {
	float x = in[0], y = in[1], z = in[2], w = in[3];
	out[0] = x * by;
	out[1] = y * by;
	out[2] = z * by;
	out[3] = w * by;
}

static void vec4_add4f(float out[4], const float in[4], float dx, float dy, float dz, float dw) {
	float x = in[0], y = in[1], z = in[2], w = in[3];
	out[0] = x + dx;
	out[1] = y + dy;
	out[2] = z + dz;
	out[3] = w + dw;
}


static void vec4_mul(float out[4], const float a[4], const float b[4]) {
	float ax = a[0], ay = a[1], az = a[2], aw = a[3];
	float bx = b[0], by = b[1], bz = b[2], bw = b[3];
	out[0] = ax * bx;
	out[1] = ay * by;
	out[2] = az * bz;
	out[3] = aw * bw;
}

static void vec4_div(float out[4], const float a[4], const float b[4]) {
	float ax = a[0], ay = a[1], az = a[2], aw = a[3];
	float bx = b[0], by = b[1], bz = b[2], bw = b[3];
	out[0] = ax / bx;
	out[1] = ay / by;
	out[2] = az / bz;
	out[3] = aw / bw;
}

static void vec4_copy(float out[4], const float in[4]) {
	out[0] = in[0];
	out[1] = in[1];
	out[2] = in[2];
	out[3] = in[3];
}

static void vec4_abs(float out[4], const float in[4]) {
	float x = in[0], y = in[1], z = in[2], w = in[3];
	out[0] = fabsf(x);
	out[1] = fabsf(y);
	out[2] = fabsf(z);
	out[3] = fabsf(w);
}

static void vec2i_set(int out[2], int x, int y) {
	out[0] = x;
	out[1] = y;
}

static void vec2i_add(int out[2], const int a[2], const int b[2]) {
	int ax = a[0], ay = a[1], bx = b[0], by = b[1];
	out[0] = ax + bx;
	out[1] = ay + by;
}

// out := normal of triangle p0-p1-p2
static void triangle_normal(float out[3], const float p0[3], const float p1[3], const float p2[3]) {
	float a[3], b[3];
	vec3_sub(a, p1, p0);
	vec3_sub(b, p2, p0);
	vec3_cross(out, a, b);
}

static void mat4_set(float out[16],
               float a, float b, float c, float d,
               float e, float f, float g, float h,
               float i, float j, float k, float l,
               float m, float n, float o, float p) {
	out[0] = a; out[4] = b; out[ 8] = c; out[12] = d;
	out[1] = e; out[5] = f; out[ 9] = g; out[13] = h;
	out[2] = i; out[6] = j; out[10] = k; out[14] = l;
	out[3] = m; out[7] = n; out[11] = o; out[15] = p;
}

static void mat4_set3x3(float out[16],
                  float a, float b, float c,
                  float d, float e, float f,
                  float g, float h, float i) {
	out[0] = a; out[4] = b; out[ 8] = c; out[12] = 0;
	out[1] = d; out[5] = e; out[ 9] = f; out[13] = 0;
	out[2] = g; out[6] = h; out[10] = i; out[14] = 0;
	out[3] = 0; out[7] = 0; out[11] = 0; out[15] = 1;
}

static const float MAT4_IDENTITY[16] = {
	1, 0, 0, 0,
	0, 1, 0, 0,
	0, 0, 1, 0,
	0, 0, 0, 1
};

// set out to identity matrix
static void mat4_identity(float out[16]) {
	memset(out, 0, 16*sizeof(float));
	out[0] = out[5] = out[10] = out[15] = 1;
}

static void mat4_yaw(float out[16], float yaw) {
	float c = cosf(yaw), s = sinf(yaw);
	mat4_set3x3(out,
		c,  0, s,
		0,  1, 0,
		-s, 0, c
	);
}

static void mat4_pitch(float out[16], float pitch) {
	float c = cosf(pitch), s = sinf(pitch);
	mat4_set3x3(out,
		1, 0,  0,
		0, c, -s,
		0, s,  c
	);
}

static void mat4_translate(float out[16], const float translation[3]) {
	mat4_set(out,
		1, 0, 0, translation[0],
		0, 1, 0, translation[1],
		0, 0, 1, translation[2],
		0, 0, 0, 1
	);
}

static void mat4_mul(float out[16], const float a[16], const float b[16]) {
	float *p = out;
	float acpy[16], bcpy[16];
	
	// just in case out == a or out == b
	memcpy(acpy, a, sizeof acpy);
	memcpy(bcpy, b, sizeof bcpy);
	
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j, ++p) {
			const float *as = &acpy[j];
			const float *bs = &bcpy[4*i];
			*p = as[0]*bs[0] + as[4]*bs[1] + as[8]*bs[2] + as[12]*bs[3];
		}
	}
}

static void vec4_apply_mat4(float out[4], const float mat[16], const float vec[4]) {
	float x = vec[0], y = vec[1], z = vec[2], w = vec[3];
	out[0] = mat[0] * x + mat[4] * y + mat[ 8] * z + mat[12] * w;
	out[1] = mat[1] * x + mat[5] * y + mat[ 9] * z + mat[13] * w;
	out[2] = mat[2] * x + mat[6] * y + mat[10] * z + mat[14] * w;
	out[3] = mat[3] * x + mat[7] * y + mat[11] * z + mat[15] * w;
	
}

static void vec3_apply_mat4(float out[3], const float mat[16], const float vec[3]) {
	float x = vec[0], y = vec[1], z = vec[2];
	out[0] = mat[0] * x + mat[4] * y + mat[ 8] * z + mat[12];
	out[1] = mat[1] * x + mat[5] * y + mat[ 9] * z + mat[13];
	out[2] = mat[2] * x + mat[6] * y + mat[10] * z + mat[14];
}

static void vec2_print(const float vec[2]) {
	printf("(" FLOAT_PRINTF ", " FLOAT_PRINTF ")", vec[0], vec[1]);
}

static void vec2_println(const float vec[2]) {
	vec2_print(vec);
	putchar('\n');
}

static void vec3_print(const float vec[3]) {
	printf("(" FLOAT_PRINTF ", " FLOAT_PRINTF ", " FLOAT_PRINTF ")", vec[0], vec[1], vec[2]);
}

static void vec3_println(const float vec[3]) {
	vec3_print(vec);
	putchar('\n');
}

static void vec4_print(const float vec[4]) {
	printf("(" FLOAT_PRINTF ", " FLOAT_PRINTF ", " FLOAT_PRINTF ", " FLOAT_PRINTF ")", vec[0], vec[1], vec[2], vec[3]);
}

static void vec4_println(const float vec[4]) {
	vec4_print(vec);
	putchar('\n');
}

static void vec2i_print(const int vec[2]) {
	printf("(%d, %d)", vec[0], vec[1]);
}

static void vec2i_println(const int vec[2]) {
	vec2i_print(vec);
	putchar('\n');
}

static void vec3i_print(const int vec[3]) {
	printf("(%d, %d, %d)", vec[0], vec[1], vec[2]);
}

static void vec3i_println(const int vec[3]) {
	vec3i_print(vec);
	putchar('\n');
}

static void vec4i_print(const int vec[4]) {
	printf("(%d, %d, %d, %d)", vec[0], vec[1], vec[2], vec[3]);
}

static void vec4i_println(const int vec[4]) {
	vec4i_print(vec);
	putchar('\n');
}

static void mat4_print(const float m[16]) {
	printf(
		"|%.3f %.3f %.3f %.3f|\n"
		"|%.3f %.3f %.3f %.3f|\n"
		"|%.3f %.3f %.3f %.3f|\n"
		"|%.3f %.3f %.3f %.3f|\n\n",
		m[0], m[4], m[ 8], m[12],
		m[1], m[5], m[ 9], m[13],
		m[2], m[6], m[10], m[14],
		m[3], m[7], m[11], m[15]);
}

/*
4x4 perspective matrix.
fov - field of view in radians, aspect - width:height aspect ratio, z_near/z_far - clipping planes
math stolen from gluPerspective (https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/gluPerspective.xml)
*/
static void mat4_perspective(float out[16], float fov, float aspect, float z_near, float z_far) {
	float f = 1.0f / tanf(fov / 2.0f);
	mat4_set(out,
		f/aspect, 0, 0, 0,
		0, f, 0, 0,
		0, 0, (z_far+z_near) / (z_near-z_far), (2.0f*z_far*z_near) / (z_near-z_far),
		0, 0, -1, 0
	);
}



void mat4_ortho(float out[16], float left, float right, float bottom, float top, float near_, float far_) {
	float tx = -(right + left)/(right - left);
	float ty = -(top + bottom)/(top - bottom);
	float tz = -(far_ + near_)/(far_ - near_);
	mat4_set(out,
		2.0f / (right - left), 0, 0, tx,
		0, 2.0f / (top - bottom), 0, ty,
		0, 0, -2.0f / (far_ - near_), tz,
		0, 0, 0, 1
	);
}


// 2D interpolating across a triangle
float triangle_interpolate_2d(const float p[2],
	const float a[2], float a_value, const float b[2], float b_value, const float c[2], float c_value) {
	float w_a = ((b[1] - c[1]) * (p[0] - c[0]) + (c[0] - b[0]) * (p[1] - c[1]))
		/ ((b[1] - c[1]) * (a[0] - c[0]) + (c[0] - b[0]) * (a[1] - c[1]));
	float w_b = ((c[1] - a[1]) * (p[0] - c[0]) + (a[0] - c[0]) * (p[1] - c[1]))
		/ ((b[1] - c[1]) * (a[0] - c[0]) + (c[0] - b[0]) * (a[1] - c[1]));
	float w_c = 1 - w_a - w_b;
	assert(w_a >= -0.001f && w_a <= 1.001f);
	assert(w_b >= -0.001f && w_b <= 1.001f);
	assert(w_c >= -0.001f && w_c <= 1.001f);
	return a_value * w_a + b_value * w_b + c_value * w_c;
}

static int rect4f_contains_vec2(float rx, float ry, float rw, float rh, const float p[2]) {
	float px = p[0], py = p[1];
	return px >= rx && py >= ry && px < rx + rw && py < ry + rh;
}

static void vec2_move_out_of_rect4f(float out[2], const float in[2], float rx, float ry, float rw, float rh) {
	float px = in[0], py = in[1];
	if (!(px >= rx && py >= ry && px < rx + rw && py < ry + rh)) {
		return; // not inside rect
	}
	float dleft = px - rx;
	float dright = rx + rw - px;
	float ddown = py - ry;
	float dup = ry + rh - py;
	if (dleft <= dright && dleft <= ddown && dleft <= dup)
		px = rx;
	else if (dright <= ddown && dright <= dup)
		px = rx + rw;
	else if (ddown <= dup)
		py = ry;
	else
		py = ry + rh;
	out[0] = px;
	out[1] = py;
	
}
