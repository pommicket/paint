typedef struct {
	float uv[2];
	float pos[2];
} VertexMain;

static int gl_version_major, gl_version_minor;
static int window_width, window_height;
static float window_widthf, window_heightf;
static uint64_t last_frame_time_ns, this_frame_time_ns;
static double last_frame_time, this_frame_time;
static float frame_dt;

static SDL_Window *sdl_window;
static GLProgram *program_main, *program_paint;

#define SHADER_COUNT (sizeof all_shaders / sizeof *all_shaders)

static float frame_dt;
