#define static_array_len(arr) (sizeof (arr) / sizeof *(arr))

typedef struct {
	GLuint id;
	uint64_t timestamp;
	char vfilename[32];
	char ffilename[32];
} GLProgram;

static void gl_message_callback(GLenum source, GLenum type, unsigned int id, GLenum severity,
	GLsizei length, const char *message, void *userParam) {
	static int nmessages = 0;
	(void)source; (void)type; (void)id; (void)length; (void)userParam;
	
	if (severity == GL_DEBUG_SEVERITY_NOTIFICATION) return;
	++nmessages;
	if (nmessages <= 10) {
		printf("[OpenGL] %s.\n", message);
	} else if (nmessages == 11) {
		printf("[OpenGL] (...further messages will not be displayed to avoid spamming stdout...)\n");
	}
}

/* get a number which will change if the shader files were edited */
static uint64_t gl_program_get_timestamp(GLProgram *program) {
	uint64_t timestamp = 0;
#if _WIN32
	HANDLE vh = CreateFileA(program->vfilename, GENERIC_READ, 7, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	HANDLE fh = CreateFileA(program->ffilename, GENERIC_READ, 7, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	FILETIME vt = {0}, ft = {0};
	if (vh != INVALID_HANDLE_VALUE) GetFileTime(vh, NULL, NULL, &vt);
	if (fh != INVALID_HANDLE_VALUE) GetFileTime(vh, NULL, NULL, &ft);
	timestamp += vt.dwLowDateTime | (uint64_t)vt.dwHighDateTime << 32;
	timestamp += ft.dwLowDateTime | (uint64_t)ft.dwHighDateTime << 32;
	if (vh != INVALID_HANDLE_VALUE) CloseHandle(vh);
	if (fh != INVALID_HANDLE_VALUE) CloseHandle(fh);
#else
	struct stat vstatbuf = {0}, fstatbuf = {0};
	if (stat(program->vfilename, &vstatbuf) == 0
	 && stat(program->ffilename, &fstatbuf) == 0) {
	 	struct timespec vt = vstatbuf.st_mtim, ft = fstatbuf.st_mtim;
		timestamp  = (uint64_t)vt.tv_sec * 1000000000 + (uint64_t)vt.tv_nsec;
		timestamp += (uint64_t)ft.tv_sec * 1000000000 + (uint64_t)ft.tv_nsec;
	}
#endif
	return timestamp;
}

static void gl_program_reload(GLProgram *program) {
	static char buf[16384];
	const char *pbuf = buf;
	FILE *vfp, *ffp;
	GLuint vshader, fshader, program_id;
	
	program->timestamp = gl_program_get_timestamp(program);
	
	if (program->id) {
		glDeleteProgram(program->id);
	}
	
	vfp = fopen(program->vfilename, "rb");
	ffp = fopen(program->ffilename, "rb");
	
	if (!vfp || !ffp) {
		assert(!*"shader program files not found.");
		return;
	}
	
	
	vshader = glCreateShader(GL_VERTEX_SHADER);
	fshader = glCreateShader(GL_FRAGMENT_SHADER);
	program_id = glCreateProgram();
	
	buf[fread(buf, 1, sizeof buf - 1, vfp)] = 0;
	glShaderSource(vshader, 1, &pbuf, NULL);
	buf[fread(buf, 1, sizeof buf - 1, ffp)] = 0;
	glShaderSource(fshader, 1, &pbuf, NULL);
	
	fclose(vfp);
	fclose(ffp);
	
	glCompileShader(vshader);
	glCompileShader(fshader);
	glAttachShader(program_id, vshader);
	glAttachShader(program_id, fshader);
	glLinkProgram(program_id);
	
	program->id = program_id;
	if (glObjectLabel) glObjectLabel(GL_PROGRAM, program_id, -1, program->vfilename);
}

static void gl_program_load(GLProgram *program, const char *vfilename, const char *ffilename) {
	memset(program, 0, sizeof *program);
	if (strlen(vfilename) >= sizeof program->vfilename
	 || strlen(ffilename) >= sizeof program->ffilename) {
		assert(!*"shader program file names too long.");
		return;
	}
	strcpy(program->vfilename, vfilename);
	strcpy(program->ffilename, ffilename);
	
	gl_program_reload(program);
}

#define GL_BATCH_SIZE 200

static GLuint gl_texture_new(const char *label) {
	static GLuint textures[GL_BATCH_SIZE], textures_used = GL_BATCH_SIZE;
	if (textures_used >= GL_BATCH_SIZE) {
		glGenTextures(GL_BATCH_SIZE, textures);
		textures_used = 0;
	}
	GLuint id = textures[textures_used++];
	//if (glObjectLabel && label) glObjectLabel(GL_TEXTURE, id, -1, label);//opengl doesn't seem to like this, at least on my computer
	return id;
}

typedef struct {
	GLuint id;
	GLsizei item_size;
	GLsizei n_items;
} GLBuffer;

static void gl_buffer_create(GLBuffer *buffer, const char *label) {
	static GLuint buffers[GL_BATCH_SIZE], buffers_used = GL_BATCH_SIZE;
	GLuint id;
	memset(buffer, 0, sizeof *buffer);
	if (buffers_used >= GL_BATCH_SIZE) {
		glGenBuffers(GL_BATCH_SIZE, buffers);
		buffers_used = 0;
	}
	id = buffers[buffers_used++];
	if (glObjectLabel && label) glObjectLabel(GL_BUFFER, id, -1, label);
	buffer->id = id;
}

static void gl_buffer_static_data(GLBuffer *buffer, const void *data, size_t n_items, size_t item_size) {
	assert(buffer->id);
	assert(n_items);
	
	if (n_items > INT_MAX / item_size) {
		debug_printf("Buffer data too large\n");
		return;
	}
	glBindBuffer(GL_ARRAY_BUFFER, buffer->id);
	glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)(item_size * n_items), data, GL_STATIC_DRAW);
	assert(!buffer->item_size || item_size == (size_t)buffer->item_size);
	buffer->item_size = (GLsizei)item_size;
	buffer->n_items = (GLsizei)n_items;
}

static void gl_buffer_stream_data(GLBuffer *buffer, const void *data, size_t n_items, size_t item_size) {
	assert(buffer->id);
	assert(n_items);
	if (n_items > INT_MAX / item_size) {
		debug_printf("Buffer data too large\n");
		return;
	}
	glBindBuffer(GL_ARRAY_BUFFER, buffer->id);
	glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)(item_size * n_items), data, GL_STREAM_DRAW);
	assert(!buffer->item_size || item_size == (size_t)buffer->item_size);
	buffer->item_size = (GLsizei)item_size;
	buffer->n_items = (GLsizei)n_items;
}

static void gl_buffer_delete(GLBuffer *buffer) {
	glDeleteBuffers(1, &buffer->id);
	memset(buffer, 0, sizeof *buffer);
}

typedef struct {
	GLuint id;
	GLuint program_id;
	GLsizei n_vertices;
} GLArray;

static void gl_array_create(GLArray *array, const GLProgram *program, const char *label) {
	static GLuint arrays[GL_BATCH_SIZE], arrays_used = GL_BATCH_SIZE;
	GLuint id;
	assert(program->id);
	memset(array, 0, sizeof *array);
	if (arrays_used >= GL_BATCH_SIZE) {
		glGenVertexArrays(GL_BATCH_SIZE, arrays);
		arrays_used = 0;
	}
	id = arrays[arrays_used++];
	if (glObjectLabel && label) glObjectLabel(GL_VERTEX_ARRAY, id, -1, label);
	array->id = id;
	array->program_id = program->id;
}

static void gl_array_delete(GLArray *array) {
	glDeleteVertexArrays(1, &array->id);
	memset(array, 0, sizeof *array);
}
static GLuint gl_bound_program;

static void gl_program_use(GLProgram *program) {
	if (program->timestamp && program->timestamp != gl_program_get_timestamp(program)) {
		gl_program_reload(program);
	}
	gl_bound_program = program->id;
	glUseProgram(program->id);
}

static void gl_vertex_attribnf(GLArray *array, GLBuffer *buffer, const char *name, size_t offset, int n) {
	assert(array->id);
	glBindVertexArray(array->id);
	glBindBuffer(GL_ARRAY_BUFFER, buffer->id);
	GLint loc = glGetAttribLocation(array->program_id, name);
	if (loc == -1) return;
	glVertexAttribPointer((GLuint)loc, n, GL_FLOAT, 0, buffer->item_size, (const GLvoid *)offset);
	glEnableVertexAttribArray((GLuint)loc);
	if (buffer->n_items) {
		assert(!array->n_vertices || array->n_vertices == buffer->n_items);
		array->n_vertices = buffer->n_items;
	}
}
static void gl_vertex_attribni(GLArray *array, GLBuffer *buffer, const char *name, size_t offset, int n) {
	assert(array->id);
	glBindVertexArray(array->id);
	glBindBuffer(GL_ARRAY_BUFFER, buffer->id);
	GLint loc = glGetAttribLocation(array->program_id, name);
	if (loc == -1) return;
	glVertexAttribIPointer((GLuint)loc, n, GL_INT, buffer->item_size, (const GLvoid *)offset);
	glEnableVertexAttribArray((GLuint)loc);
	if (buffer->n_items) {
		assert(!array->n_vertices || array->n_vertices == buffer->n_items);
		array->n_vertices = buffer->n_items;
	}
}

static void gl_vertex_attrib1f(GLArray *array, GLBuffer *buffer, const char *name, size_t offset) {
	gl_vertex_attribnf(array, buffer, name, offset, 1);
}
static void gl_vertex_attrib2f(GLArray *array, GLBuffer *buffer, const char *name, size_t offset) {
	gl_vertex_attribnf(array, buffer, name, offset, 2);
}
static void gl_vertex_attrib3f(GLArray *array, GLBuffer *buffer, const char *name, size_t offset) {
	gl_vertex_attribnf(array, buffer, name, offset, 3);
}
static void gl_vertex_attrib4f(GLArray *array, GLBuffer *buffer, const char *name, size_t offset) {
	gl_vertex_attribnf(array, buffer, name, offset, 4);
}

static void gl_vertex_attrib1i(GLArray *array, GLBuffer *buffer, const char *name, size_t offset) {
	gl_vertex_attribni(array, buffer, name, offset, 1);
}
static void gl_vertex_attrib2i(GLArray *array, GLBuffer *buffer, const char *name, size_t offset) {
	gl_vertex_attribni(array, buffer, name, offset, 2);
}
static void gl_vertex_attrib3i(GLArray *array, GLBuffer *buffer, const char *name, size_t offset) {
	gl_vertex_attribni(array, buffer, name, offset, 3);
}
static void gl_vertex_attrib4i(GLArray *array, GLBuffer *buffer, const char *name, size_t offset) {
	gl_vertex_attribni(array, buffer, name, offset, 4);
}

// NB: gl_vertex_attrib* will set the number of vertices to buffer->n_items.
// this function should only really be used if you want to set up an array before setting up the corresponding vertex buffer.
static void gl_array_set_nvertices(GLArray *array, int n) {
	array->n_vertices = n;
}

// call this before changing number of vertices array refers to
static void gl_array_clear(GLArray *array) {
	array->n_vertices = 0;
}

static void gl_uniform_mat4(const char *name, const float *value) {
	assert(gl_bound_program);
	glUniformMatrix4fv(glGetUniformLocation(gl_bound_program, name), 1, 0, value);
}
static void gl_uniform1i(const char *name, int x) {
	assert(gl_bound_program);
	glUniform1i(glGetUniformLocation(gl_bound_program, name), x);
}
static void gl_uniform2i(const char *name, int x, int y) {
	assert(gl_bound_program);
	glUniform2i(glGetUniformLocation(gl_bound_program, name), x, y);
}
static void gl_uniform3i(const char *name, int x, int y, int z) {
	assert(gl_bound_program);
	glUniform3i(glGetUniformLocation(gl_bound_program, name), x, y, z);
}
static void gl_uniform4i(const char *name, int x, int y, int z, int w) {
	assert(gl_bound_program);
	glUniform4i(glGetUniformLocation(gl_bound_program, name), x, y, z, w);
}
static void gl_uniform1f(const char *name, float x) {
	assert(gl_bound_program);
	glUniform1f(glGetUniformLocation(gl_bound_program, name), x);
}
static void gl_uniform2f(const char *name, float x, float y) {
	assert(gl_bound_program);
	glUniform2f(glGetUniformLocation(gl_bound_program, name), x, y);
}
static void gl_uniform3f(const char *name, float x, float y, float z) {
	assert(gl_bound_program);
	glUniform3f(glGetUniformLocation(gl_bound_program, name), x, y, z);
}
static void gl_uniform4f(const char *name, float x, float y, float z, float w) {
	assert(gl_bound_program);
	glUniform4f(glGetUniformLocation(gl_bound_program, name), x, y, z, w);
}
static void gl_uniform_vec2(const char *name, const float value[2]) {
	gl_uniform2f(name, value[0], value[1]);
}
static void gl_uniform_vec3(const char *name, const float value[3]) {
	gl_uniform3f(name, value[0], value[1], value[2]);
}
static void gl_uniform_vec4(const char *name, const float value[4]) {
	gl_uniform4f(name, value[0], value[1], value[2], value[3]);
}

static void gl_array_draw(GLArray *array) {
	//assert(array->program_id == gl_bound_program);
	assert(array->n_vertices);
	glBindVertexArray(array->id);
	glDrawArrays(GL_TRIANGLES, 0, array->n_vertices);
}

static void gl_array_draw_with_index_buffer(GLArray *array, GLBuffer *indices) {
	//assert(array->program_id == gl_bound_program);
	glBindVertexArray(array->id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices->id);
	glDrawElements(GL_TRIANGLES, (GLsizei)indices->n_items, GL_UNSIGNED_INT, NULL);
}


static void gl_array_draw_with_indices(GLArray *array, const uint32_t *indices, size_t count) {
	//assert(array->program_id == gl_bound_program);
	assert(count < INT_MAX);
	glBindVertexArray(array->id);
	glDrawElements(GL_TRIANGLES, (GLsizei)count, GL_UNSIGNED_INT, indices);
}

static void gl_active_texture(int slot, GLuint texture) {
	assert(slot >= 0 && slot < 32);
	glActiveTexture(GL_TEXTURE0 + (GLenum)slot);
	glBindTexture(GL_TEXTURE_2D, texture);
}

static void gl_active_texture_ms(int slot, GLuint texture) {
	assert(slot >= 0 && slot < 32);
	glActiveTexture(GL_TEXTURE0 + (GLenum)slot);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, texture);
}

#ifdef FI_PNG_H_
typedef struct {
	GLenum min_filter;
	GLenum mag_filter;
	bool mipmap;
} GLTexConfig;
static const GLTexConfig gl_tex_config_default = {GL_NEAREST, GL_LINEAR, false};

static GLuint gl_png_data_to_texture(PNGData *data, const char *label, const GLTexConfig *config) {
	GLuint texture;
	
	if (!config)
		config = &gl_tex_config_default;
	
	png_data_convert(data, 4);
	if (!data->success) {
	#if DEBUG
		printf("png error: %s\n", data->error);
	#endif
		return 0;
	}
	texture = gl_texture_new(label);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (GLsizei)data->width, (GLsizei)data->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data->pixels);
	if (config->mipmap) {
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (GLint)config->min_filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (GLint)config->mag_filter);
	/* why is repeat the default.... */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	return texture;
}
#endif
