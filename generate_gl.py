#!/usr/bin/python3

filename = 'gldef.c'

# function_str format: (min gl version).return_type.glName.arguments
function_str = '''
void.glViewport.GLint x, GLint y, GLsizei width, GLsizei height
void.glClear.GLbitfield mask
void.glClearColor.GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha
void.glClearDepth.GLdouble depth
void.glClearStencil.GLint s
GLuint.glCreateShader.GLenum type
GLuint.glCreateProgram.void
void.glShaderSource.GLuint shader, GLsizei count, const GLchar **string, const GLint *length
void.glCompileShader.GLuint shader
void.glAttachShader.GLuint program, GLuint shader
void.glLinkProgram.GLuint program
void.glGetShaderiv.GLuint shader, GLenum pname, GLint *params
void.glGetShaderInfoLog.GLuint shader, GLsizei maxLength, GLsizei *length, GLchar *infoLog
void.glGetProgramiv.GLuint program, GLenum pname, GLint *params
void.glGetProgramInfoLog.GLuint program, GLsizei maxLength, GLsizei *length, GLchar *infoLog
void.glEnable.GLenum cap
void.glDisable.GLenum cap
void.glPolygonMode.GLenum face, GLenum mode
void.glCullFace.GLenum mode
void.glBlendFunc.GLenum sfactor, GLenum dfactor
void.glDepthFunc.GLenum func
void.glGenTextures.GLsizei n, GLuint *textures
void.glGetIntegerv.GLenum pname, GLint *data
403.void.glDebugMessageCallback.GLDEBUGPROC callback, void *user
void.glGenBuffers.GLsizei n, GLuint *buffers
300.void.glGenVertexArrays.GLsizei n, GLuint *arrays
300.void.glBindVertexArray.GLuint array
void.glBindBuffer.GLenum target, GLuint buffer
void.glBufferData.GLenum target, GLsizeiptr size, const GLvoid *data, GLenum usage
void.glTexImage2D.GLenum target, GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *data
void.glTexParameteri.GLenum target, GLenum pname, GLint param
300.void.glGenerateMipmap.GLenum target
void.glBindTexture.GLenum target, GLuint texture
void.glActiveTexture.GLenum texture
403.void.glObjectLabel.GLenum identifier, GLuint name, GLsizei length, const char *label
403.void.glGetObjectLabel.GLenum identifier, GLuint name, GLsizei bifSize, GLsizei *length, char *label
void.glUseProgram.GLuint program
GLint.glGetUniformLocation.GLuint program, const GLchar *name
GLint.glGetAttribLocation.GLuint program, const GLchar *name
void.glVertexAttribPointer.GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid * pointer
300.void.glVertexAttribIPointer.GLuint index, GLint size, GLenum type, GLsizei stride, const GLvoid *pointer
void.glEnableVertexAttribArray.GLuint index
void.glUniform1f.GLint location, GLfloat v0
void.glUniform2f.GLint location, GLfloat v0, GLfloat v1
void.glUniform3f.GLint location, GLfloat v0, GLfloat v1, GLfloat v2
void.glUniform4f.GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3
300.void.glUniform1i.GLint location, GLint v0
300.void.glUniform2i.GLint location, GLint v0, GLint v1
300.void.glUniform3i.GLint location, GLint v0, GLint v1, GLint v2
300.void.glUniform4i.GLint location, GLint v0, GLint v1, GLint v2, GLint v3
void.glUniformMatrix4fv.GLint location, GLsizei count, GLboolean transpose, const GLfloat *value
void.glDeleteBuffers.GLsizei n, const GLuint *buffers
void.glDeleteVertexArrays.GLsizei n, const GLuint *arrays
void.glDeleteProgram.GLuint program
void.glDeleteShader.GLuint shader
void.glDeleteTextures.GLsizei n, const GLuint *textures
void.glDetachShader.GLuint program, GLuint shader
void.glDrawArrays.GLenum mode, GLint first, GLsizei count
void.glDrawElements.GLenum mode, GLsizei count, GLenum type, const GLvoid *indices
GLenum.glGetError.void
300.void.glGenFramebuffers.GLsizei n, GLuint *ids
300.void.glDeleteFramebuffers.GLsizei n, GLuint *framebuffers
300.void.glFramebufferTexture2D.GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level
300.void.glFramebufferRenderbuffer.GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer
300.void.glBindFramebuffer.GLenum target, GLuint framebuffer
320.void.glTexImage2DMultisample.GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations
void.glReadPixels.GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid * data
'''

functions = []
for line in function_str.split('\n'):
	if line:
		parts = line.split('.')
		if len(parts) == 3:
			functions.append([0] + parts)
		else:
			functions.append([int(parts[0])] + parts[1:])

defines = '''
GL_COLOR_BUFFER_BIT=0x4000
GL_DEPTH_BUFFER_BIT=0x0100
GL_FRAGMENT_SHADER=0x8b30
GL_VERTEX_SHADER=0x8b31
GL_DEBUG_OUTPUT=0x92E0
GL_DEBUG_OUTPUT_SYNCHRONOUS=0x8242
GL_CONTEXT_FLAG_DEBUG_BIT=0x00000002
GL_CULL_FACE=0x0B44
GL_DEPTH_TEST=0x0B71
GL_BLEND=0x0BE2
GL_ARRAY_BUFFER=0x8892
GL_STATIC_DRAW=0x88e4
GL_STREAM_DRAW=0x88e0
GL_CONTEXT_FLAGS=0x821e
GL_FRONT=0x404
GL_BACK=0x405
GL_FRONT_AND_BACK=0x408
GL_LINE=0x1B01
GL_FILL=0x1B02
GL_RED=0x1903
GL_RGB=0x1907
GL_RGBA=0x1908
GL_TEXTURE_2D=0xde1
GL_UNSIGNED_BYTE=0x1401
GL_BYTE=0x1400
GL_UNSIGNED_SHORT=0x1403
GL_SHORT=0x1402
GL_UNSIGNED_INT=0x1405
GL_INT=0x1404
GL_FLOAT=0x1406
GL_BUFFER=0x82e0
GL_VERTEX_ARRAY=0x8074
GL_PROGRAM=0x82e2
GL_ELEMENT_ARRAY_BUFFER=0x8893
GL_COMPILE_STATUS=0x8b81
GL_LINK_STATUS=0x8b82
GL_DEBUG_SEVERITY_NOTIFICATION=0x826B
GL_LINES=1
GL_TRIANGLES=4
GL_SRC_ALPHA=0x0302
GL_ONE_MINUS_SRC_ALPHA=0x0303
GL_ONE=1
GL_ZERO=0
GL_LESS=0x0201
GL_GREATER=0x0204
GL_LEQUAL=0x0203
GL_GEQUAL=0x0206
GL_EQUAL=0x0202
GL_NOTEQUAL=0x0205
GL_NEVER=0x0200
GL_ALWAYS=0x0207
GL_DONT_CARE=0x1100
GL_ELEMENT_ARRAY_BUFFER=0x8893
GL_TEXTURE=0x1702
GL_RED=0x1903
GL_RG=0x8227
GL_RGB=0x1907
GL_RGBA=0x1908
GL_BGRA=0x80e1
GL_R8=0x8229
GL_RG8=0x822B
GL_RGB8=0x8051
GL_RGBA8=0x8058
GL_TEXTURE_MAG_FILTER=0x2800
GL_TEXTURE_MIN_FILTER=0x2801
GL_TEXTURE_WRAP_S=0x2802
GL_TEXTURE_WRAP_T=0x2803
GL_CLAMP_TO_EDGE=0x812F
GL_CLAMP_TO_BORDER=0x812D
GL_REPEAT=0x2901
GL_MIRRORED_REPEAT=0x8370
GL_NEAREST=0x2600
GL_LINEAR=0x2601
GL_NEAREST_MIPMAP_NEAREST=0x2700
GL_LINEAR_MIPMAP_LINEAR=0x2703
GL_LINEAR_MIPMAP_NEAREST=0x2701
GL_NEAREST_MIPMAP_LINEAR=0x2702
GL_R8=0x8229
GL_R16=0x822A
GL_RG8=0x822B
GL_RG16=0x822C
GL_R16F=0x822D
GL_R32F=0x822E
GL_RG16F=0x822F
GL_RG32F=0x8230
GL_R8I=0x8231
GL_R8UI=0x8232
GL_R16I=0x8233
GL_R16UI=0x8234
GL_R32I=0x8235
GL_R32UI=0x8236
GL_RG8I=0x8237
GL_RG8UI=0x8238
GL_RG16I=0x8239
GL_RG16UI=0x823A
GL_RG32I=0x823B
GL_RG32UI=0x823C
GL_DEPTH_COMPONENT32F=0x8CAC
GL_DEPTH32F_STENCIL8=0x8CAD
GL_FLOAT_32_UNSIGNED_INT_24_8_REV=0x8DAD
GL_INVALID_FRAMEBUFFER_OPERATION=0x0506
GL_FRAMEBUFFER_ATTACHMENT_COLOR_ENCODING=0x8210
GL_FRAMEBUFFER_ATTACHMENT_COMPONENT_TYPE=0x8211
GL_FRAMEBUFFER_ATTACHMENT_RED_SIZE=0x8212
GL_FRAMEBUFFER_ATTACHMENT_GREEN_SIZE=0x8213
GL_FRAMEBUFFER_ATTACHMENT_BLUE_SIZE=0x8214
GL_FRAMEBUFFER_ATTACHMENT_ALPHA_SIZE=0x8215
GL_FRAMEBUFFER_ATTACHMENT_DEPTH_SIZE=0x8216
GL_FRAMEBUFFER_ATTACHMENT_STENCIL_SIZE=0x8217
GL_FRAMEBUFFER_DEFAULT=0x8218
GL_FRAMEBUFFER_UNDEFINED=0x8219
GL_DEPTH_STENCIL_ATTACHMENT=0x821A
GL_MAX_RENDERBUFFER_SIZE=0x84E8
GL_DEPTH_STENCIL=0x84F9
GL_UNSIGNED_INT_24_8=0x84FA
GL_DEPTH24_STENCIL8=0x88F0
GL_TEXTURE_STENCIL_SIZE=0x88F1
GL_TEXTURE_RED_TYPE=0x8C10
GL_TEXTURE_GREEN_TYPE=0x8C11
GL_TEXTURE_BLUE_TYPE=0x8C12
GL_TEXTURE_ALPHA_TYPE=0x8C13
GL_TEXTURE_DEPTH_TYPE=0x8C16
GL_UNSIGNED_NORMALIZED=0x8C17
GL_FRAMEBUFFER_BINDING=0x8CA6
GL_DRAW_FRAMEBUFFER_BINDING=0x8CA6
GL_RENDERBUFFER_BINDING=0x8CA7
GL_READ_FRAMEBUFFER=0x8CA8
GL_DRAW_FRAMEBUFFER=0x8CA9
GL_READ_FRAMEBUFFER_BINDING=0x8CAA
GL_RENDERBUFFER_SAMPLES=0x8CAB
GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE=0x8CD0
GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME=0x8CD1
GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL=0x8CD2
GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE=0x8CD3
GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LAYER=0x8CD4
GL_FRAMEBUFFER_COMPLETE=0x8CD5
GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT=0x8CD6
GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT=0x8CD7
GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER=0x8CDB
GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER=0x8CDC
GL_FRAMEBUFFER_UNSUPPORTED=0x8CDD
GL_MAX_COLOR_ATTACHMENTS=0x8CDF
GL_COLOR_ATTACHMENT0=0x8CE0
GL_COLOR_ATTACHMENT1=0x8CE1
GL_COLOR_ATTACHMENT2=0x8CE2
GL_COLOR_ATTACHMENT3=0x8CE3
GL_COLOR_ATTACHMENT4=0x8CE4
GL_COLOR_ATTACHMENT5=0x8CE5
GL_COLOR_ATTACHMENT6=0x8CE6
GL_COLOR_ATTACHMENT7=0x8CE7
GL_COLOR_ATTACHMENT8=0x8CE8
GL_COLOR_ATTACHMENT9=0x8CE9
GL_COLOR_ATTACHMENT10=0x8CEA
GL_COLOR_ATTACHMENT11=0x8CEB
GL_COLOR_ATTACHMENT12=0x8CEC
GL_COLOR_ATTACHMENT13=0x8CED
GL_COLOR_ATTACHMENT14=0x8CEE
GL_COLOR_ATTACHMENT15=0x8CEF
GL_COLOR_ATTACHMENT16=0x8CF0
GL_COLOR_ATTACHMENT17=0x8CF1
GL_COLOR_ATTACHMENT18=0x8CF2
GL_COLOR_ATTACHMENT19=0x8CF3
GL_COLOR_ATTACHMENT20=0x8CF4
GL_COLOR_ATTACHMENT21=0x8CF5
GL_COLOR_ATTACHMENT22=0x8CF6
GL_COLOR_ATTACHMENT23=0x8CF7
GL_COLOR_ATTACHMENT24=0x8CF8
GL_COLOR_ATTACHMENT25=0x8CF9
GL_COLOR_ATTACHMENT26=0x8CFA
GL_COLOR_ATTACHMENT27=0x8CFB
GL_COLOR_ATTACHMENT28=0x8CFC
GL_COLOR_ATTACHMENT29=0x8CFD
GL_COLOR_ATTACHMENT30=0x8CFE
GL_COLOR_ATTACHMENT31=0x8CFF
GL_DEPTH_ATTACHMENT=0x8D00
GL_STENCIL_ATTACHMENT=0x8D20
GL_FRAMEBUFFER=0x8D40
GL_RENDERBUFFER=0x8D41
GL_RENDERBUFFER_WIDTH=0x8D42
GL_RENDERBUFFER_HEIGHT=0x8D43
GL_RENDERBUFFER_INTERNAL_FORMAT=0x8D44
GL_STENCIL_INDEX1=0x8D46
GL_STENCIL_INDEX4=0x8D47
GL_STENCIL_INDEX8=0x8D48
GL_STENCIL_INDEX16=0x8D49
GL_RENDERBUFFER_RED_SIZE=0x8D50
GL_RENDERBUFFER_GREEN_SIZE=0x8D51
GL_RENDERBUFFER_BLUE_SIZE=0x8D52
GL_RENDERBUFFER_ALPHA_SIZE=0x8D53
GL_RENDERBUFFER_DEPTH_SIZE=0x8D54
GL_RENDERBUFFER_STENCIL_SIZE=0x8D55
GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE=0x8D56
GL_MAX_SAMPLES=0x8D57
GL_FRAMEBUFFER_SRGB=0x8DB9
GL_HALF_FLOAT=0x140B
GL_TEXTURE_2D_MULTISAMPLE=0x9100
GL_RGBA16F=0x881A
GL_RGB16F=0x881B
GL_STENCIL=0x1802
GL_STENCIL_INDEX=0x1901
GL_DEPTH_COMPONENT=0x1902
'''



f = open(filename, 'w')

f.write('''// File auto-generated by generate_gl.py

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
''')

for line in defines.split('\n'):
	if line == '': continue
	name, value = line.split('=')
	f.write('#define {} {}\n'.format(name, value))
for i in range(32):
	f.write('#define GL_TEXTURE{} 0x{:x}\n'.format(i, 0x84c0+i))
f.write('\n')

def functype(f):
	return f[:2].upper() + f[2:]

for vers, ret, name, args in functions:
	f.write('typedef {} {}({});\n'.format(ret, functype(name), args))

f.write('\n')

for vers, ret, name, args in functions:
	f.write('static {} *{};\n'.format(functype(name), name))

f.write('''
void gl_get_procs(int version_major, int version_minor) {
	int version = version_major * 100 + version_minor;
''')
for vers, ret, name, args in functions:
	if vers:
		f.write('\tif (version >= {}) '.format(vers))
	else:
		f.write('\t')
	f.write('{} = ({} *)SDL_GL_GetProcAddress("{}");\n'.format(name, functype(name), name))
f.write('''}

''')

f.close()
