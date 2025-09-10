#ifndef __glad_h_
#define __glad_h_

#ifdef __gl_h_
#error OpenGL header already included, remove this include, glad already provides it
#endif
#define __gl_h_

#if defined(_WIN32) && !defined(APIENTRY) && !defined(__CYGWIN__) && !defined(__SCITECH_SNAP__)
#define APIENTRY __stdcall
#endif

#ifndef APIENTRY
#define APIENTRY
#endif
#ifndef APIENTRYP
#define APIENTRYP APIENTRY *
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifndef GLAPI
#define GLAPI extern
#endif

struct gladGLversionStruct {
    int major;
    int minor;
};

typedef void* (* GLADloadproc)(const char *name);

GLAPI struct gladGLversionStruct GLVersion;
GLAPI int gladLoadGL(void);
GLAPI int gladLoadGLLoader(GLADloadproc);

#include <KHR/khrplatform.h>
typedef unsigned int GLenum;
typedef unsigned char GLboolean;
typedef unsigned int GLbitfield;
typedef void GLvoid;
typedef khronos_int8_t GLbyte;
typedef khronos_uint8_t GLubyte;
typedef khronos_int16_t GLshort;
typedef khronos_uint16_t GLushort;
typedef int GLint;
typedef unsigned int GLuint;
typedef khronos_int32_t GLclampx;
typedef int GLsizei;
typedef khronos_float_t GLfloat;
typedef khronos_float_t GLclampf;
typedef double GLdouble;
typedef double GLclampd;
typedef void *GLeglClientBufferEXT;
typedef void *GLeglImageOES;
typedef char GLchar;
typedef char GLcharARB;
typedef unsigned int GLhandleARB;
typedef khronos_uint16_t GLhalf;
typedef khronos_uint16_t GLhalfARB;
typedef khronos_int32_t GLfixed;
typedef khronos_intptr_t GLintptr;
typedef khronos_intptr_t GLintptrARB;
typedef khronos_ssize_t GLsizeiptr;
typedef khronos_ssize_t GLsizeiptrARB;
typedef khronos_int64_t GLint64;
typedef khronos_int64_t GLint64EXT;
typedef khronos_uint64_t GLuint64;
typedef khronos_uint64_t GLuint64EXT;
typedef struct __GLsync *GLsync;
struct _cl_context;
struct _cl_event;
typedef void (APIENTRY *GLDEBUGPROC)(GLenum source,GLenum type,GLuint id,GLenum severity,GLsizei length,const GLchar *message,const void *userParam);
typedef void (APIENTRY *GLDEBUGPROCARB)(GLenum source,GLenum type,GLuint id,GLenum severity,GLsizei length,const GLchar *message,const void *userParam);
typedef void (APIENTRY *GLDEBUGPROCKHR)(GLenum source,GLenum type,GLuint id,GLenum severity,GLsizei length,const GLchar *message,const void *userParam);

#define GL_FALSE 0
#define GL_TRUE 1
#define GL_NONE 0
#define GL_ZERO 0
#define GL_ONE 1
#define GL_NO_ERROR 0
#define GL_POINTS 0x0000
#define GL_LINES 0x0001
#define GL_LINE_LOOP 0x0002
#define GL_LINE_STRIP 0x0003
#define GL_TRIANGLES 0x0004
#define GL_TRIANGLE_STRIP 0x0005
#define GL_TRIANGLE_FAN 0x0006
#define GL_NEVER 0x0200
#define GL_LESS 0x0201
#define GL_EQUAL 0x0202
#define GL_LEQUAL 0x0203
#define GL_GREATER 0x0204
#define GL_NOTEQUAL 0x0205
#define GL_GEQUAL 0x0206
#define GL_ALWAYS 0x0207
#define GL_SRC_COLOR 0x0300
#define GL_ONE_MINUS_SRC_COLOR 0x0301
#define GL_SRC_ALPHA 0x0302
#define GL_ONE_MINUS_SRC_ALPHA 0x0303
#define GL_DST_ALPHA 0x0304
#define GL_ONE_MINUS_DST_ALPHA 0x0305
#define GL_DST_COLOR 0x0306
#define GL_ONE_MINUS_DST_COLOR 0x0307
#define GL_SRC_ALPHA_SATURATE 0x0308
#define GL_FRONT 0x0404
#define GL_BACK 0x0405
#define GL_FRONT_AND_BACK 0x0408
#define GL_INVALID_ENUM 0x0500
#define GL_INVALID_VALUE 0x0501
#define GL_INVALID_OPERATION 0x0502
#define GL_OUT_OF_MEMORY 0x0505
#define GL_CW 0x0900
#define GL_CCW 0x0901
#define GL_CULL_FACE 0x0B44
#define GL_DEPTH_TEST 0x0B71
#define GL_STENCIL_TEST 0x0B90
#define GL_VIEWPORT 0x0BA2
#define GL_BLEND 0x0BE2
#define GL_SCISSOR_TEST 0x0C11
#define GL_TEXTURE_2D 0x0DE1
#define GL_BYTE 0x1400
#define GL_UNSIGNED_BYTE 0x1401
#define GL_SHORT 0x1402
#define GL_UNSIGNED_SHORT 0x1403
#define GL_INT 0x1404
#define GL_UNSIGNED_INT 0x1405
#define GL_FLOAT 0x1406
#define GL_DOUBLE 0x140A
#define GL_COLOR_BUFFER_BIT 0x00004000
#define GL_DEPTH_BUFFER_BIT 0x00000100
#define GL_STENCIL_BUFFER_BIT 0x00000400
#define GL_FRAMEBUFFER 0x8D40
#define GL_RENDERBUFFER 0x8D41
#define GL_ARRAY_BUFFER 0x8892
#define GL_ELEMENT_ARRAY_BUFFER 0x8893
#define GL_STATIC_DRAW 0x88E4
#define GL_DYNAMIC_DRAW 0x88E8
#define GL_FRAGMENT_SHADER 0x8B30
#define GL_VERTEX_SHADER 0x8B31
#define GL_COMPILE_STATUS 0x8B81
#define GL_LINK_STATUS 0x8B82
#define GL_INFO_LOG_LENGTH 0x8B84
#define GL_TEXTURE0 0x84C0
#define GL_BGRA 0x80E1
#define GL_RGB 0x1907
#define GL_RGBA 0x1908

GLAPI void (APIENTRYP glClearColor)(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
GLAPI void (APIENTRYP glClear)(GLbitfield mask);
GLAPI void (APIENTRYP glViewport)(GLint x, GLint y, GLsizei width, GLsizei height);
GLAPI void (APIENTRYP glEnable)(GLenum cap);
GLAPI void (APIENTRYP glDisable)(GLenum cap);
GLAPI void (APIENTRYP glBlendFunc)(GLenum sfactor, GLenum dfactor);
GLAPI void (APIENTRYP glDepthFunc)(GLenum func);
GLAPI void (APIENTRYP glDrawArrays)(GLenum mode, GLint first, GLsizei count);
GLAPI void (APIENTRYP glDrawElements)(GLenum mode, GLsizei count, GLenum type, const GLvoid *indices);
GLAPI void (APIENTRYP glDrawArraysInstanced)(GLenum mode, GLint first, GLsizei count, GLsizei primcount);
GLAPI void (APIENTRYP glGetIntegerv)(GLenum pname, GLint *params);
GLAPI GLenum (APIENTRYP glGetError)(void);
GLAPI const GLubyte* (APIENTRYP glGetString)(GLenum name);

GLAPI GLuint (APIENTRYP glCreateShader)(GLenum type);
GLAPI void (APIENTRYP glShaderSource)(GLuint shader, GLsizei count, const GLchar *const *string, const GLint *length);
GLAPI void (APIENTRYP glCompileShader)(GLuint shader);
GLAPI void (APIENTRYP glGetShaderiv)(GLuint shader, GLenum pname, GLint *params);
GLAPI void (APIENTRYP glGetShaderInfoLog)(GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
GLAPI void (APIENTRYP glDeleteShader)(GLuint shader);

GLAPI GLuint (APIENTRYP glCreateProgram)(void);
GLAPI void (APIENTRYP glAttachShader)(GLuint program, GLuint shader);
GLAPI void (APIENTRYP glLinkProgram)(GLuint program);
GLAPI void (APIENTRYP glGetProgramiv)(GLuint program, GLenum pname, GLint *params);
GLAPI void (APIENTRYP glGetProgramInfoLog)(GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
GLAPI void (APIENTRYP glUseProgram)(GLuint program);
GLAPI void (APIENTRYP glDeleteProgram)(GLuint program);

GLAPI GLint (APIENTRYP glGetUniformLocation)(GLuint program, const GLchar *name);
GLAPI void (APIENTRYP glUniform1i)(GLint location, GLint v0);
GLAPI void (APIENTRYP glUniform1f)(GLint location, GLfloat v0);
GLAPI void (APIENTRYP glUniform3fv)(GLint location, GLsizei count, const GLfloat *value);
GLAPI void (APIENTRYP glUniformMatrix4fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);

GLAPI void (APIENTRYP glGenBuffers)(GLsizei n, GLuint *buffers);
GLAPI void (APIENTRYP glBindBuffer)(GLenum target, GLuint buffer);
GLAPI void (APIENTRYP glBufferData)(GLenum target, GLsizeiptr size, const GLvoid *data, GLenum usage);
GLAPI void (APIENTRYP glBufferSubData)(GLenum target, GLintptr offset, GLsizeiptr size, const GLvoid *data);
GLAPI void (APIENTRYP glDeleteBuffers)(GLsizei n, const GLuint *buffers);

GLAPI void (APIENTRYP glGenVertexArrays)(GLsizei n, GLuint *arrays);
GLAPI void (APIENTRYP glBindVertexArray)(GLuint array);
GLAPI void (APIENTRYP glDeleteVertexArrays)(GLsizei n, const GLuint *arrays);
GLAPI void (APIENTRYP glVertexAttribPointer)(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *pointer);
GLAPI void (APIENTRYP glEnableVertexAttribArray)(GLuint index);
GLAPI void (APIENTRYP glDisableVertexAttribArray)(GLuint index);
GLAPI void (APIENTRYP glVertexAttribDivisor)(GLuint index, GLuint divisor);

GLAPI void (APIENTRYP glGenTextures)(GLsizei n, GLuint *textures);
GLAPI void (APIENTRYP glBindTexture)(GLenum target, GLuint texture);
GLAPI void (APIENTRYP glTexImage2D)(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
GLAPI void (APIENTRYP glTexParameteri)(GLenum target, GLenum pname, GLint param);
GLAPI void (APIENTRYP glActiveTexture)(GLenum texture);
GLAPI void (APIENTRYP glDeleteTextures)(GLsizei n, const GLuint *textures);

#ifdef __cplusplus
}
#endif

#endif