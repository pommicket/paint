/*
  a very simple painting program
  this program is released into the public domain
  
  press q/e to change pen size, w/s to change pen hue
  automatically saves a new image when you close the window
  you can load an image by providing a command line argument
*/
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

#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb_image_resize.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include "fi_png.c"
#include "os.c"
#include "util.c"
#include "gldef.c"
#include "gl.c"
#include "base.h"

void hsv_to_rgb(const float hsv[3], float rgb[3]) {
	float h = hsv[0], s = hsv[1], v = hsv[2];
	float c = v * s;
	float H = h * 6;
	float x = c * (1 - fabsf(fmodf(H, 2) - 1));
	float r1=0, g1=0, b1=0;
	if (H < 1)
		r1=c, g1=x, b1=0;
	else if (H < 2)
		r1=x, g1=c, b1=0;
	else if (H < 3)
		r1=0, g1=c, b1=x;
	else if (H < 4)
		r1=0, g1=x, b1=c;
	else if (H < 5)
		r1=x, g1=0, b1=c;
	else
		r1=c, g1=0, b1=x;
	float m = v - c;
	rgb[0] = r1 + m;
	rgb[1] = g1 + m;
	rgb[2] = b1 + m;
}

void flip_image(void *image, int width, int height) {
	void *temp_row = calloc((size_t)width, 4);
	
	// flip image vertically
	for (int y = 0; y < height / 2; ++y) {
		char *line1 = (char *)image + y * width * 4;
		char *line2 = (char *)image + (height-1-y) * width * 4;
		memcpy(temp_row, line1, width * 4);
		memcpy(line1, line2, width * 4);
		memcpy(line2, temp_row, width * 4);
	}
	free(temp_row);
}

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
	
	void *tex_data[2] = {0};
	size_t area = (size_t)width * (size_t)height;
	void *image = tex_data[0] = calloc(area, 4);
	PNGData data = {0};

	if (argc > 1) {
		char *input_filename = argv[1];
		png_read(input_filename, &data);
		png_data_convert(&data, 4);
		if (!data.success) {
			printf("Error reading image %s: %s\n", input_filename, data.error);
		} else {
			stbir_resize_uint8(data.pixels, data.width, data.height, 0,
				image, width, height, 0, 4);
			flip_image(image, width, height);
		}
	}
	
	if (!data.success) {
		uint32_t *p = image;
		for (size_t i = 0; i < area; ++i)
			*p++ = 0xff000000;
	}
	
	{ // make framebuffer
		glGenFramebuffers(1, &fbo);
		glGenTextures(2, color_textures);
		for (int i = 0; i < 2; ++i) {
			glBindTexture(GL_TEXTURE_2D, color_textures[i]);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex_data[i]);
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
	
	float pen_size = 20;
	float pen_hue = 0;
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
			case SDL_MOUSEWHEEL:
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
		{
			float pen_size_change_rate = 2.0f;
			float pen_hue_change_rate = 0.5f;
			if (keyboard_state[SDL_SCANCODE_Q])
				pen_size *= powf(pen_size_change_rate, frame_dt);
			if (keyboard_state[SDL_SCANCODE_A])
				pen_size /= powf(pen_size_change_rate, frame_dt);
			if (keyboard_state[SDL_SCANCODE_W])
				pen_hue += frame_dt * pen_hue_change_rate;
			if (keyboard_state[SDL_SCANCODE_S])
				pen_hue -= frame_dt * pen_hue_change_rate;
			pen_hue = fmodf(pen_hue + 1, 1);
			pen_size = clampf(pen_size, 0.1f, 5000);
		}
		
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
		
		float pen_color[4] = {0,0,0,1};
		float pen_hsv[3] = {pen_hue, 1, 1};
		hsv_to_rgb(pen_hsv, pen_color);
		
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		
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
	
	glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo);
	glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, image);
	
	SDL_Quit();
	
	flip_image(image, width, height);
	
	char filename[64] = {0};
	time_t curr_time = time(NULL);
	struct tm *tm = localtime(&curr_time);
	strftime(filename, sizeof filename - 1, "%Y-%m-%d-%H:%M:%S.png", tm);
	stbi_write_png(filename, width, height, 4, image, width * 4);
	
	SDL_GL_DeleteContext(glctx);
	
	return 0;
}
