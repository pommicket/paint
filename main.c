#ifdef DEBUG
#undef DEBUG
#undef NDEBUG
#define DEBUG 1
#else
#undef NDEBUG
#define NDEBUG 1
#endif
#include <stdio.h>
#if DEBUG
#define SDL_DISABLE_IMMINTRIN_H // speed up debug compile time
#endif
#if __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wimplicit-fallthrough"
#endif
// SDL doesnt seem to actually make an entry point on windows i cant fucking get it to work
#define SDL_MAIN_HANDLED
#include <SDL.h>
#if __clang__
#pragma clang diagnostic pop
#endif
#include <signal.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <assert.h>
#include <limits.h>
#include <float.h>

#include "fi_png.c"
#include "os.c"
#include "util.c"
#include "gldef.c"
#include "gl.c"
#include "base.h"

#if _WIN32
int WinMain(
  HINSTANCE hInstance,
  HINSTANCE hPrevInstance,
  LPSTR     lpCmdLine,
  int       nShowCmd
)
#else
int main(int argc, char **argv)
#endif
{
	SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER|SDL_INIT_AUDIO);
	signal(SIGINT, SIG_DFL);
	signal(SIGTERM, SIG_DFL);
	
	sdl_window = SDL_CreateWindow("paint", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		1280, 720, SDL_WINDOW_SHOWN|SDL_WINDOW_OPENGL);
	
	int versions[][2] = {
		{4,3},
		{3,2},
		//{3,0},
		{0}
	};
	SDL_GLContext glctx = 0;
	
	for (int i = 0; versions[i][0]; ++i) {
		gl_version_major = 4, gl_version_minor = 3;
		
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, gl_version_major);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, gl_version_minor);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
		
		glctx = SDL_GL_CreateContext(sdl_window);
		if (glctx) break;
	}
	if (!glctx) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "No GL context", "Couldn't get OpenGL context", NULL);
		return EXIT_FAILURE;
	}
	debug_printf("OpenGL v. %d.%d\n", gl_version_major,gl_version_minor);
	
	gl_get_procs(gl_version_major, gl_version_minor);
	
	{
		// debug context
		GLint flags = 0;
		glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
		if (flags & GL_CONTEXT_FLAG_DEBUG_BIT) {
			glEnable(GL_DEBUG_OUTPUT);
			glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
			glDebugMessageCallback(gl_message_callback, NULL);
		}
	}
	
	SDL_GL_SetSwapInterval(1); // vsync on
	
	{ // initialization
		program_main = calloc(1, sizeof *program_main);
		gl_program_load(program_main, "shaders/mainv.glsl", "shaders/mainf.glsl");
		program_paint = calloc(1, sizeof *program_paint);
		gl_program_load(program_paint, "shaders/paintv.glsl", "shaders/paintf.glsl");
	}
	
	uint64_t start_time_ns = os_get_time();
	last_frame_time_ns = start_time_ns;
	
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	GLuint fbo = 0, color_textures[2] = {0};
	int draw_texture = 0;
	
	int width = 3840;
	int height = 2160;
	
	{ // make framebuffer
		glGenFramebuffers(1, &fbo);
		glGenTextures(2, color_textures);
		for (int i = 0; i < 2; ++i) {
			glBindTexture(GL_TEXTURE_2D, color_textures[i]);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}
		
	}
	
	GLArray frame_array = {0};
	GLBuffer frame_buffer = {0};
	
	{
		VertexMain frame_vertices[4] = {
			{.pos = {-1, -1}, .uv = {0, 0}},
			{.pos = {+1, -1}, .uv = {1, 0}},
			{.pos = {+1, +1}, .uv = {1, 1}},
			{.pos = {-1, +1}, .uv = {0, 1}}
		};
		gl_buffer_create(&frame_buffer, "Frame");
		gl_array_create(&frame_array, program_main, "Frame");
		gl_buffer_static_data(&frame_buffer, frame_vertices, 4, sizeof(VertexMain));
		gl_vertex_attrib2f(&frame_array, &frame_buffer, "v_pos", offsetof(VertexMain, pos));
		gl_vertex_attrib2f(&frame_array, &frame_buffer, "v_uv", offsetof(VertexMain, uv));
	}
	
	SDL_ShowCursor(SDL_DISABLE);
	
	float pen_size = 10;
	float pen_color[4] = {1, 0, 1, 1};
	int prev_mouse_x = INT_MAX, prev_mouse_y = INT_MAX;
	
	double total_dt = 0;
	while (1) {
		SDL_Event event = {0};
		
		this_frame_time_ns = os_get_time();
		this_frame_time = (double)(this_frame_time_ns - start_time_ns) * 1e-9;
		last_frame_time = (double)(last_frame_time_ns - start_time_ns) * 1e-9;
		frame_dt = (float)(this_frame_time - last_frame_time);
		frame_dt = clampf(frame_dt, 0.001f, 0.1f);
		total_dt += frame_dt;
		
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:
				goto quit;
			case SDL_MOUSEMOTION:
				break;
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym) {
				case SDLK_ESCAPE:
					goto quit;
				}
				break;
			}
		}
		
		const Uint8 *keyboard_state = SDL_GetKeyboardState(NULL);
		(void)keyboard_state;
		
		SDL_GetWindowSize(sdl_window, &window_width, &window_height);
		window_widthf = (float)window_width;
		window_heightf = (float)window_height;
		
		int mouse_x = INT_MAX/2, mouse_y = INT_MAX/2;
		Uint32 mouse_state = SDL_GetMouseState(&mouse_x, &mouse_y);
		
		mouse_x = (mouse_x * width) / window_width;
		mouse_y = (mouse_y * height) / window_height;
		
		if (prev_mouse_x == INT_MAX) prev_mouse_x = mouse_x;
		if (prev_mouse_y == INT_MAX) prev_mouse_y = mouse_y;
		
		draw_texture = !draw_texture;
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color_textures[draw_texture], 0);
		
		gl_program_use(program_paint);
		glViewport(0, 0, width, height);
		gl_uniform2f("u_size", (float)width, (float)height);
		gl_uniform2f("u_prev_mouse_pos", (float)prev_mouse_x, (float)prev_mouse_y);
		gl_uniform2f("u_mouse_pos", (float)mouse_x, (float)mouse_y);
		gl_uniform1i("u_lmouse_down", (mouse_state & SDL_BUTTON_LMASK) != 0);
		gl_uniform1f("u_pen_size", pen_size);
		gl_uniform_vec4("u_pen_color", pen_color);
		gl_active_texture(0, color_textures[!draw_texture]);
		gl_uniform1i("u_texture", 0);
		{
			GLuint indices[] = {0,1,2,0,2,3};
			gl_array_draw_with_indices(&frame_array, indices, 6);
		}
		
		
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT);
		
		
		glViewport(0, 0, window_width, window_height);
		gl_program_use(program_main);
		gl_uniform2f("u_size", (float)width, (float)height);
		gl_uniform2f("u_mouse_pos", (float)mouse_x, (float)mouse_y);
		gl_uniform1f("u_pen_size", pen_size);
		gl_uniform_vec4("u_pen_color", pen_color);
		gl_active_texture(0, color_textures[draw_texture]);
		gl_uniform1i("u_texture", 0);
		{
			GLuint indices[] = {0,1,2,0,2,3};
			gl_array_draw_with_indices(&frame_array, indices, 6);
		}
		
		
		SDL_GL_SwapWindow(sdl_window);
		
		last_frame_time_ns = this_frame_time_ns;
		prev_mouse_x = mouse_x;
		prev_mouse_y = mouse_y;
	}
	quit:
	
	SDL_GL_DeleteContext(glctx);
	
	return 0;
}
