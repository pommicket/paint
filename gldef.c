// File auto-generated by generate_gl.py

typedef float GLfloat;
typedef double GLdouble;
typedef char GLchar;
typedef unsigned char GLuchar;
typedef unsigned char GLboolean;
typedef int GLint;
typedef unsigned GLuint;
typedef void GLvoid;
typedef unsigned GLbitfield;
typedef unsigned GLenum;
typedef int GLsizei;
typedef
#if _WIN64
long long
#else
long
#endif
GLsizeiptr;
typedef void (*GLDEBUGPROC)(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
	const GLchar *message, void *user);
#define GL_COLOR_BUFFER_BIT 0x4000
#define GL_DEPTH_BUFFER_BIT 0x0100
#define GL_FRAGMENT_SHADER 0x8b30
#define GL_VERTEX_SHADER 0x8b31
#define GL_DEBUG_OUTPUT 0x92E0
#define GL_DEBUG_OUTPUT_SYNCHRONOUS 0x8242
#define GL_CONTEXT_FLAG_DEBUG_BIT 0x00000002
#define GL_CULL_FACE 0x0B44
#define GL_DEPTH_TEST 0x0B71
#define GL_BLEND 0x0BE2
#define GL_ARRAY_BUFFER 0x8892
#define GL_STATIC_DRAW 0x88e4
#define GL_STREAM_DRAW 0x88e0
#define GL_CONTEXT_FLAGS 0x821e
#define GL_FRONT 0x404
#define GL_BACK 0x405
#define GL_FRONT_AND_BACK 0x408
#define GL_LINE 0x1B01
#define GL_FILL 0x1B02
#define GL_RED 0x1903
#define GL_RGB 0x1907
#define GL_RGBA 0x1908
#define GL_TEXTURE_2D 0xde1
#define GL_UNSIGNED_BYTE 0x1401
#define GL_BYTE 0x1400
#define GL_UNSIGNED_SHORT 0x1403
#define GL_SHORT 0x1402
#define GL_UNSIGNED_INT 0x1405
#define GL_INT 0x1404
#define GL_FLOAT 0x1406
#define GL_BUFFER 0x82e0
#define GL_VERTEX_ARRAY 0x8074
#define GL_PROGRAM 0x82e2
#define GL_ELEMENT_ARRAY_BUFFER 0x8893
#define GL_COMPILE_STATUS 0x8b81
#define GL_LINK_STATUS 0x8b82
#define GL_DEBUG_SEVERITY_NOTIFICATION 0x826B
#define GL_LINES 1
#define GL_TRIANGLES 4
#define GL_SRC_ALPHA 0x0302
#define GL_ONE_MINUS_SRC_ALPHA 0x0303
#define GL_ONE 1
#define GL_ZERO 0
#define GL_LESS 0x0201
#define GL_GREATER 0x0204
#define GL_LEQUAL 0x0203
#define GL_GEQUAL 0x0206
#define GL_EQUAL 0x0202
#define GL_NOTEQUAL 0x0205
#define GL_NEVER 0x0200
#define GL_ALWAYS 0x0207
#define GL_DONT_CARE 0x1100
#define GL_ELEMENT_ARRAY_BUFFER 0x8893
#define GL_TEXTURE 0x1702
#define GL_RED 0x1903
#define GL_RG 0x8227
#define GL_RGB 0x1907
#define GL_RGBA 0x1908
#define GL_BGRA 0x80e1
#define GL_R8 0x8229
#define GL_RG8 0x822B
#define GL_RGB8 0x8051
#define GL_RGBA8 0x8058
#define GL_TEXTURE_MAG_FILTER 0x2800
#define GL_TEXTURE_MIN_FILTER 0x2801
#define GL_TEXTURE_WRAP_S 0x2802
#define GL_TEXTURE_WRAP_T 0x2803
#define GL_CLAMP_TO_EDGE 0x812F
#define GL_CLAMP_TO_BORDER 0x812D
#define GL_REPEAT 0x2901
#define GL_MIRRORED_REPEAT 0x8370
#define GL_NEAREST 0x2600
#define GL_LINEAR 0x2601
#define GL_NEAREST_MIPMAP_NEAREST 0x2700
#define GL_LINEAR_MIPMAP_LINEAR 0x2703
#define GL_LINEAR_MIPMAP_NEAREST 0x2701
#define GL_NEAREST_MIPMAP_LINEAR 0x2702
#define GL_R8 0x8229
#define GL_R16 0x822A
#define GL_RG8 0x822B
#define GL_RG16 0x822C
#define GL_R16F 0x822D
#define GL_R32F 0x822E
#define GL_RG16F 0x822F
#define GL_RG32F 0x8230
#define GL_R8I 0x8231
#define GL_R8UI 0x8232
#define GL_R16I 0x8233
#define GL_R16UI 0x8234
#define GL_R32I 0x8235
#define GL_R32UI 0x8236
#define GL_RG8I 0x8237
#define GL_RG8UI 0x8238
#define GL_RG16I 0x8239
#define GL_RG16UI 0x823A
#define GL_RG32I 0x823B
#define GL_RG32UI 0x823C
#define GL_DEPTH_COMPONENT32F 0x8CAC
#define GL_DEPTH32F_STENCIL8 0x8CAD
#define GL_FLOAT_32_UNSIGNED_INT_24_8_REV 0x8DAD
#define GL_INVALID_FRAMEBUFFER_OPERATION 0x0506
#define GL_FRAMEBUFFER_ATTACHMENT_COLOR_ENCODING 0x8210
#define GL_FRAMEBUFFER_ATTACHMENT_COMPONENT_TYPE 0x8211
#define GL_FRAMEBUFFER_ATTACHMENT_RED_SIZE 0x8212
#define GL_FRAMEBUFFER_ATTACHMENT_GREEN_SIZE 0x8213
#define GL_FRAMEBUFFER_ATTACHMENT_BLUE_SIZE 0x8214
#define GL_FRAMEBUFFER_ATTACHMENT_ALPHA_SIZE 0x8215
#define GL_FRAMEBUFFER_ATTACHMENT_DEPTH_SIZE 0x8216
#define GL_FRAMEBUFFER_ATTACHMENT_STENCIL_SIZE 0x8217
#define GL_FRAMEBUFFER_DEFAULT 0x8218
#define GL_FRAMEBUFFER_UNDEFINED 0x8219
#define GL_DEPTH_STENCIL_ATTACHMENT 0x821A
#define GL_MAX_RENDERBUFFER_SIZE 0x84E8
#define GL_DEPTH_STENCIL 0x84F9
#define GL_UNSIGNED_INT_24_8 0x84FA
#define GL_DEPTH24_STENCIL8 0x88F0
#define GL_TEXTURE_STENCIL_SIZE 0x88F1
#define GL_TEXTURE_RED_TYPE 0x8C10
#define GL_TEXTURE_GREEN_TYPE 0x8C11
#define GL_TEXTURE_BLUE_TYPE 0x8C12
#define GL_TEXTURE_ALPHA_TYPE 0x8C13
#define GL_TEXTURE_DEPTH_TYPE 0x8C16
#define GL_UNSIGNED_NORMALIZED 0x8C17
#define GL_FRAMEBUFFER_BINDING 0x8CA6
#define GL_DRAW_FRAMEBUFFER_BINDING 0x8CA6
#define GL_RENDERBUFFER_BINDING 0x8CA7
#define GL_READ_FRAMEBUFFER 0x8CA8
#define GL_DRAW_FRAMEBUFFER 0x8CA9
#define GL_READ_FRAMEBUFFER_BINDING 0x8CAA
#define GL_RENDERBUFFER_SAMPLES 0x8CAB
#define GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE 0x8CD0
#define GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME 0x8CD1
#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL 0x8CD2
#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE 0x8CD3
#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LAYER 0x8CD4
#define GL_FRAMEBUFFER_COMPLETE 0x8CD5
#define GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT 0x8CD6
#define GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT 0x8CD7
#define GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER 0x8CDB
#define GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER 0x8CDC
#define GL_FRAMEBUFFER_UNSUPPORTED 0x8CDD
#define GL_MAX_COLOR_ATTACHMENTS 0x8CDF
#define GL_COLOR_ATTACHMENT0 0x8CE0
#define GL_COLOR_ATTACHMENT1 0x8CE1
#define GL_COLOR_ATTACHMENT2 0x8CE2
#define GL_COLOR_ATTACHMENT3 0x8CE3
#define GL_COLOR_ATTACHMENT4 0x8CE4
#define GL_COLOR_ATTACHMENT5 0x8CE5
#define GL_COLOR_ATTACHMENT6 0x8CE6
#define GL_COLOR_ATTACHMENT7 0x8CE7
#define GL_COLOR_ATTACHMENT8 0x8CE8
#define GL_COLOR_ATTACHMENT9 0x8CE9
#define GL_COLOR_ATTACHMENT10 0x8CEA
#define GL_COLOR_ATTACHMENT11 0x8CEB
#define GL_COLOR_ATTACHMENT12 0x8CEC
#define GL_COLOR_ATTACHMENT13 0x8CED
#define GL_COLOR_ATTACHMENT14 0x8CEE
#define GL_COLOR_ATTACHMENT15 0x8CEF
#define GL_COLOR_ATTACHMENT16 0x8CF0
#define GL_COLOR_ATTACHMENT17 0x8CF1
#define GL_COLOR_ATTACHMENT18 0x8CF2
#define GL_COLOR_ATTACHMENT19 0x8CF3
#define GL_COLOR_ATTACHMENT20 0x8CF4
#define GL_COLOR_ATTACHMENT21 0x8CF5
#define GL_COLOR_ATTACHMENT22 0x8CF6
#define GL_COLOR_ATTACHMENT23 0x8CF7
#define GL_COLOR_ATTACHMENT24 0x8CF8
#define GL_COLOR_ATTACHMENT25 0x8CF9
#define GL_COLOR_ATTACHMENT26 0x8CFA
#define GL_COLOR_ATTACHMENT27 0x8CFB
#define GL_COLOR_ATTACHMENT28 0x8CFC
#define GL_COLOR_ATTACHMENT29 0x8CFD
#define GL_COLOR_ATTACHMENT30 0x8CFE
#define GL_COLOR_ATTACHMENT31 0x8CFF
#define GL_DEPTH_ATTACHMENT 0x8D00
#define GL_STENCIL_ATTACHMENT 0x8D20
#define GL_FRAMEBUFFER 0x8D40
#define GL_RENDERBUFFER 0x8D41
#define GL_RENDERBUFFER_WIDTH 0x8D42
#define GL_RENDERBUFFER_HEIGHT 0x8D43
#define GL_RENDERBUFFER_INTERNAL_FORMAT 0x8D44
#define GL_STENCIL_INDEX1 0x8D46
#define GL_STENCIL_INDEX4 0x8D47
#define GL_STENCIL_INDEX8 0x8D48
#define GL_STENCIL_INDEX16 0x8D49
#define GL_RENDERBUFFER_RED_SIZE 0x8D50
#define GL_RENDERBUFFER_GREEN_SIZE 0x8D51
#define GL_RENDERBUFFER_BLUE_SIZE 0x8D52
#define GL_RENDERBUFFER_ALPHA_SIZE 0x8D53
#define GL_RENDERBUFFER_DEPTH_SIZE 0x8D54
#define GL_RENDERBUFFER_STENCIL_SIZE 0x8D55
#define GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE 0x8D56
#define GL_MAX_SAMPLES 0x8D57
#define GL_FRAMEBUFFER_SRGB 0x8DB9
#define GL_HALF_FLOAT 0x140B
#define GL_TEXTURE_2D_MULTISAMPLE 0x9100
#define GL_RGBA16F 0x881A
#define GL_RGB16F 0x881B
#define GL_STENCIL 0x1802
#define GL_STENCIL_INDEX 0x1901
#define GL_DEPTH_COMPONENT 0x1902
#define GL_TEXTURE0 0x84c0
#define GL_TEXTURE1 0x84c1
#define GL_TEXTURE2 0x84c2
#define GL_TEXTURE3 0x84c3
#define GL_TEXTURE4 0x84c4
#define GL_TEXTURE5 0x84c5
#define GL_TEXTURE6 0x84c6
#define GL_TEXTURE7 0x84c7
#define GL_TEXTURE8 0x84c8
#define GL_TEXTURE9 0x84c9
#define GL_TEXTURE10 0x84ca
#define GL_TEXTURE11 0x84cb
#define GL_TEXTURE12 0x84cc
#define GL_TEXTURE13 0x84cd
#define GL_TEXTURE14 0x84ce
#define GL_TEXTURE15 0x84cf
#define GL_TEXTURE16 0x84d0
#define GL_TEXTURE17 0x84d1
#define GL_TEXTURE18 0x84d2
#define GL_TEXTURE19 0x84d3
#define GL_TEXTURE20 0x84d4
#define GL_TEXTURE21 0x84d5
#define GL_TEXTURE22 0x84d6
#define GL_TEXTURE23 0x84d7
#define GL_TEXTURE24 0x84d8
#define GL_TEXTURE25 0x84d9
#define GL_TEXTURE26 0x84da
#define GL_TEXTURE27 0x84db
#define GL_TEXTURE28 0x84dc
#define GL_TEXTURE29 0x84dd
#define GL_TEXTURE30 0x84de
#define GL_TEXTURE31 0x84df

typedef void GLViewport(GLint x, GLint y, GLsizei width, GLsizei height);
typedef void GLClear(GLbitfield mask);
typedef void GLClearColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
typedef void GLClearDepth(GLdouble depth);
typedef void GLClearStencil(GLint s);
typedef GLuint GLCreateShader(GLenum type);
typedef GLuint GLCreateProgram(void);
typedef void GLShaderSource(GLuint shader, GLsizei count, const GLchar **string, const GLint *length);
typedef void GLCompileShader(GLuint shader);
typedef void GLAttachShader(GLuint program, GLuint shader);
typedef void GLLinkProgram(GLuint program);
typedef void GLGetShaderiv(GLuint shader, GLenum pname, GLint *params);
typedef void GLGetShaderInfoLog(GLuint shader, GLsizei maxLength, GLsizei *length, GLchar *infoLog);
typedef void GLGetProgramiv(GLuint program, GLenum pname, GLint *params);
typedef void GLGetProgramInfoLog(GLuint program, GLsizei maxLength, GLsizei *length, GLchar *infoLog);
typedef void GLEnable(GLenum cap);
typedef void GLDisable(GLenum cap);
typedef void GLPolygonMode(GLenum face, GLenum mode);
typedef void GLCullFace(GLenum mode);
typedef void GLBlendFunc(GLenum sfactor, GLenum dfactor);
typedef void GLDepthFunc(GLenum func);
typedef void GLGenTextures(GLsizei n, GLuint *textures);
typedef void GLGetIntegerv(GLenum pname, GLint *data);
typedef void GLDebugMessageCallback(GLDEBUGPROC callback, void *user);
typedef void GLGenBuffers(GLsizei n, GLuint *buffers);
typedef void GLGenVertexArrays(GLsizei n, GLuint *arrays);
typedef void GLBindVertexArray(GLuint array);
typedef void GLBindBuffer(GLenum target, GLuint buffer);
typedef void GLBufferData(GLenum target, GLsizeiptr size, const GLvoid *data, GLenum usage);
typedef void GLTexImage2D(GLenum target, GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *data);
typedef void GLTexParameteri(GLenum target, GLenum pname, GLint param);
typedef void GLGenerateMipmap(GLenum target);
typedef void GLBindTexture(GLenum target, GLuint texture);
typedef void GLActiveTexture(GLenum texture);
typedef void GLObjectLabel(GLenum identifier, GLuint name, GLsizei length, const char *label);
typedef void GLGetObjectLabel(GLenum identifier, GLuint name, GLsizei bifSize, GLsizei *length, char *label);
typedef void GLUseProgram(GLuint program);
typedef GLint GLGetUniformLocation(GLuint program, const GLchar *name);
typedef GLint GLGetAttribLocation(GLuint program, const GLchar *name);
typedef void GLVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid * pointer);
typedef void GLVertexAttribIPointer(GLuint index, GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
typedef void GLEnableVertexAttribArray(GLuint index);
typedef void GLUniform1f(GLint location, GLfloat v0);
typedef void GLUniform2f(GLint location, GLfloat v0, GLfloat v1);
typedef void GLUniform3f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
typedef void GLUniform4f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
typedef void GLUniform1i(GLint location, GLint v0);
typedef void GLUniform2i(GLint location, GLint v0, GLint v1);
typedef void GLUniform3i(GLint location, GLint v0, GLint v1, GLint v2);
typedef void GLUniform4i(GLint location, GLint v0, GLint v1, GLint v2, GLint v3);
typedef void GLUniformMatrix4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void GLDeleteBuffers(GLsizei n, const GLuint *buffers);
typedef void GLDeleteVertexArrays(GLsizei n, const GLuint *arrays);
typedef void GLDeleteProgram(GLuint program);
typedef void GLDeleteShader(GLuint shader);
typedef void GLDeleteTextures(GLsizei n, const GLuint *textures);
typedef void GLDetachShader(GLuint program, GLuint shader);
typedef void GLDrawArrays(GLenum mode, GLint first, GLsizei count);
typedef void GLDrawElements(GLenum mode, GLsizei count, GLenum type, const GLvoid *indices);
typedef GLenum GLGetError(void);
typedef void GLGenFramebuffers(GLsizei n, GLuint *ids);
typedef void GLDeleteFramebuffers(GLsizei n, GLuint *framebuffers);
typedef void GLFramebufferTexture2D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
typedef void GLFramebufferRenderbuffer(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
typedef void GLBindFramebuffer(GLenum target, GLuint framebuffer);
typedef void GLTexImage2DMultisample(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations);
typedef void GLReadPixels(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid * data);

static GLViewport *glViewport;
static GLClear *glClear;
static GLClearColor *glClearColor;
static GLClearDepth *glClearDepth;
static GLClearStencil *glClearStencil;
static GLCreateShader *glCreateShader;
static GLCreateProgram *glCreateProgram;
static GLShaderSource *glShaderSource;
static GLCompileShader *glCompileShader;
static GLAttachShader *glAttachShader;
static GLLinkProgram *glLinkProgram;
static GLGetShaderiv *glGetShaderiv;
static GLGetShaderInfoLog *glGetShaderInfoLog;
static GLGetProgramiv *glGetProgramiv;
static GLGetProgramInfoLog *glGetProgramInfoLog;
static GLEnable *glEnable;
static GLDisable *glDisable;
static GLPolygonMode *glPolygonMode;
static GLCullFace *glCullFace;
static GLBlendFunc *glBlendFunc;
static GLDepthFunc *glDepthFunc;
static GLGenTextures *glGenTextures;
static GLGetIntegerv *glGetIntegerv;
static GLDebugMessageCallback *glDebugMessageCallback;
static GLGenBuffers *glGenBuffers;
static GLGenVertexArrays *glGenVertexArrays;
static GLBindVertexArray *glBindVertexArray;
static GLBindBuffer *glBindBuffer;
static GLBufferData *glBufferData;
static GLTexImage2D *glTexImage2D;
static GLTexParameteri *glTexParameteri;
static GLGenerateMipmap *glGenerateMipmap;
static GLBindTexture *glBindTexture;
static GLActiveTexture *glActiveTexture;
static GLObjectLabel *glObjectLabel;
static GLGetObjectLabel *glGetObjectLabel;
static GLUseProgram *glUseProgram;
static GLGetUniformLocation *glGetUniformLocation;
static GLGetAttribLocation *glGetAttribLocation;
static GLVertexAttribPointer *glVertexAttribPointer;
static GLVertexAttribIPointer *glVertexAttribIPointer;
static GLEnableVertexAttribArray *glEnableVertexAttribArray;
static GLUniform1f *glUniform1f;
static GLUniform2f *glUniform2f;
static GLUniform3f *glUniform3f;
static GLUniform4f *glUniform4f;
static GLUniform1i *glUniform1i;
static GLUniform2i *glUniform2i;
static GLUniform3i *glUniform3i;
static GLUniform4i *glUniform4i;
static GLUniformMatrix4fv *glUniformMatrix4fv;
static GLDeleteBuffers *glDeleteBuffers;
static GLDeleteVertexArrays *glDeleteVertexArrays;
static GLDeleteProgram *glDeleteProgram;
static GLDeleteShader *glDeleteShader;
static GLDeleteTextures *glDeleteTextures;
static GLDetachShader *glDetachShader;
static GLDrawArrays *glDrawArrays;
static GLDrawElements *glDrawElements;
static GLGetError *glGetError;
static GLGenFramebuffers *glGenFramebuffers;
static GLDeleteFramebuffers *glDeleteFramebuffers;
static GLFramebufferTexture2D *glFramebufferTexture2D;
static GLFramebufferRenderbuffer *glFramebufferRenderbuffer;
static GLBindFramebuffer *glBindFramebuffer;
static GLTexImage2DMultisample *glTexImage2DMultisample;
static GLReadPixels *glReadPixels;

void gl_get_procs(int version_major, int version_minor) {
	int version = version_major * 100 + version_minor;
	glViewport = (GLViewport *)SDL_GL_GetProcAddress("glViewport");
	glClear = (GLClear *)SDL_GL_GetProcAddress("glClear");
	glClearColor = (GLClearColor *)SDL_GL_GetProcAddress("glClearColor");
	glClearDepth = (GLClearDepth *)SDL_GL_GetProcAddress("glClearDepth");
	glClearStencil = (GLClearStencil *)SDL_GL_GetProcAddress("glClearStencil");
	glCreateShader = (GLCreateShader *)SDL_GL_GetProcAddress("glCreateShader");
	glCreateProgram = (GLCreateProgram *)SDL_GL_GetProcAddress("glCreateProgram");
	glShaderSource = (GLShaderSource *)SDL_GL_GetProcAddress("glShaderSource");
	glCompileShader = (GLCompileShader *)SDL_GL_GetProcAddress("glCompileShader");
	glAttachShader = (GLAttachShader *)SDL_GL_GetProcAddress("glAttachShader");
	glLinkProgram = (GLLinkProgram *)SDL_GL_GetProcAddress("glLinkProgram");
	glGetShaderiv = (GLGetShaderiv *)SDL_GL_GetProcAddress("glGetShaderiv");
	glGetShaderInfoLog = (GLGetShaderInfoLog *)SDL_GL_GetProcAddress("glGetShaderInfoLog");
	glGetProgramiv = (GLGetProgramiv *)SDL_GL_GetProcAddress("glGetProgramiv");
	glGetProgramInfoLog = (GLGetProgramInfoLog *)SDL_GL_GetProcAddress("glGetProgramInfoLog");
	glEnable = (GLEnable *)SDL_GL_GetProcAddress("glEnable");
	glDisable = (GLDisable *)SDL_GL_GetProcAddress("glDisable");
	glPolygonMode = (GLPolygonMode *)SDL_GL_GetProcAddress("glPolygonMode");
	glCullFace = (GLCullFace *)SDL_GL_GetProcAddress("glCullFace");
	glBlendFunc = (GLBlendFunc *)SDL_GL_GetProcAddress("glBlendFunc");
	glDepthFunc = (GLDepthFunc *)SDL_GL_GetProcAddress("glDepthFunc");
	glGenTextures = (GLGenTextures *)SDL_GL_GetProcAddress("glGenTextures");
	glGetIntegerv = (GLGetIntegerv *)SDL_GL_GetProcAddress("glGetIntegerv");
	if (version >= 403) glDebugMessageCallback = (GLDebugMessageCallback *)SDL_GL_GetProcAddress("glDebugMessageCallback");
	glGenBuffers = (GLGenBuffers *)SDL_GL_GetProcAddress("glGenBuffers");
	if (version >= 300) glGenVertexArrays = (GLGenVertexArrays *)SDL_GL_GetProcAddress("glGenVertexArrays");
	if (version >= 300) glBindVertexArray = (GLBindVertexArray *)SDL_GL_GetProcAddress("glBindVertexArray");
	glBindBuffer = (GLBindBuffer *)SDL_GL_GetProcAddress("glBindBuffer");
	glBufferData = (GLBufferData *)SDL_GL_GetProcAddress("glBufferData");
	glTexImage2D = (GLTexImage2D *)SDL_GL_GetProcAddress("glTexImage2D");
	glTexParameteri = (GLTexParameteri *)SDL_GL_GetProcAddress("glTexParameteri");
	if (version >= 300) glGenerateMipmap = (GLGenerateMipmap *)SDL_GL_GetProcAddress("glGenerateMipmap");
	glBindTexture = (GLBindTexture *)SDL_GL_GetProcAddress("glBindTexture");
	glActiveTexture = (GLActiveTexture *)SDL_GL_GetProcAddress("glActiveTexture");
	if (version >= 403) glObjectLabel = (GLObjectLabel *)SDL_GL_GetProcAddress("glObjectLabel");
	if (version >= 403) glGetObjectLabel = (GLGetObjectLabel *)SDL_GL_GetProcAddress("glGetObjectLabel");
	glUseProgram = (GLUseProgram *)SDL_GL_GetProcAddress("glUseProgram");
	glGetUniformLocation = (GLGetUniformLocation *)SDL_GL_GetProcAddress("glGetUniformLocation");
	glGetAttribLocation = (GLGetAttribLocation *)SDL_GL_GetProcAddress("glGetAttribLocation");
	glVertexAttribPointer = (GLVertexAttribPointer *)SDL_GL_GetProcAddress("glVertexAttribPointer");
	if (version >= 300) glVertexAttribIPointer = (GLVertexAttribIPointer *)SDL_GL_GetProcAddress("glVertexAttribIPointer");
	glEnableVertexAttribArray = (GLEnableVertexAttribArray *)SDL_GL_GetProcAddress("glEnableVertexAttribArray");
	glUniform1f = (GLUniform1f *)SDL_GL_GetProcAddress("glUniform1f");
	glUniform2f = (GLUniform2f *)SDL_GL_GetProcAddress("glUniform2f");
	glUniform3f = (GLUniform3f *)SDL_GL_GetProcAddress("glUniform3f");
	glUniform4f = (GLUniform4f *)SDL_GL_GetProcAddress("glUniform4f");
	if (version >= 300) glUniform1i = (GLUniform1i *)SDL_GL_GetProcAddress("glUniform1i");
	if (version >= 300) glUniform2i = (GLUniform2i *)SDL_GL_GetProcAddress("glUniform2i");
	if (version >= 300) glUniform3i = (GLUniform3i *)SDL_GL_GetProcAddress("glUniform3i");
	if (version >= 300) glUniform4i = (GLUniform4i *)SDL_GL_GetProcAddress("glUniform4i");
	glUniformMatrix4fv = (GLUniformMatrix4fv *)SDL_GL_GetProcAddress("glUniformMatrix4fv");
	glDeleteBuffers = (GLDeleteBuffers *)SDL_GL_GetProcAddress("glDeleteBuffers");
	glDeleteVertexArrays = (GLDeleteVertexArrays *)SDL_GL_GetProcAddress("glDeleteVertexArrays");
	glDeleteProgram = (GLDeleteProgram *)SDL_GL_GetProcAddress("glDeleteProgram");
	glDeleteShader = (GLDeleteShader *)SDL_GL_GetProcAddress("glDeleteShader");
	glDeleteTextures = (GLDeleteTextures *)SDL_GL_GetProcAddress("glDeleteTextures");
	glDetachShader = (GLDetachShader *)SDL_GL_GetProcAddress("glDetachShader");
	glDrawArrays = (GLDrawArrays *)SDL_GL_GetProcAddress("glDrawArrays");
	glDrawElements = (GLDrawElements *)SDL_GL_GetProcAddress("glDrawElements");
	glGetError = (GLGetError *)SDL_GL_GetProcAddress("glGetError");
	if (version >= 300) glGenFramebuffers = (GLGenFramebuffers *)SDL_GL_GetProcAddress("glGenFramebuffers");
	if (version >= 300) glDeleteFramebuffers = (GLDeleteFramebuffers *)SDL_GL_GetProcAddress("glDeleteFramebuffers");
	if (version >= 300) glFramebufferTexture2D = (GLFramebufferTexture2D *)SDL_GL_GetProcAddress("glFramebufferTexture2D");
	if (version >= 300) glFramebufferRenderbuffer = (GLFramebufferRenderbuffer *)SDL_GL_GetProcAddress("glFramebufferRenderbuffer");
	if (version >= 300) glBindFramebuffer = (GLBindFramebuffer *)SDL_GL_GetProcAddress("glBindFramebuffer");
	if (version >= 320) glTexImage2DMultisample = (GLTexImage2DMultisample *)SDL_GL_GetProcAddress("glTexImage2DMultisample");
	glReadPixels = (GLReadPixels *)SDL_GL_GetProcAddress("glReadPixels");
}

