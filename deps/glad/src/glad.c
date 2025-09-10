#include <glad/glad.h>
#include <string.h>
#include <stdlib.h>

#ifdef _WIN32
#include <windows.h>
static HMODULE libGL;

static void* get_proc(const char *namez) {
    void* result = (void*)wglGetProcAddress(namez);
    if(result == 0 || (result == (void*)0x1) || (result == (void*)0x2) || (result == (void*)0x3) || (result == (void*)-1)) {
        result = (void*)GetProcAddress(libGL, namez);
    }
    return result;
}
#else
#include <dlfcn.h>
static void* libGL;

static void* get_proc(const char *namez) {
    return dlsym(libGL, namez);
}
#endif

struct gladGLversionStruct GLVersion = { 0, 0 };

void (APIENTRYP glClearColor)(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
void (APIENTRYP glClear)(GLbitfield mask);
void (APIENTRYP glViewport)(GLint x, GLint y, GLsizei width, GLsizei height);
void (APIENTRYP glEnable)(GLenum cap);
void (APIENTRYP glDisable)(GLenum cap);
void (APIENTRYP glBlendFunc)(GLenum sfactor, GLenum dfactor);
void (APIENTRYP glDepthFunc)(GLenum func);
void (APIENTRYP glDrawArrays)(GLenum mode, GLint first, GLsizei count);
void (APIENTRYP glDrawElements)(GLenum mode, GLsizei count, GLenum type, const GLvoid *indices);
void (APIENTRYP glDrawArraysInstanced)(GLenum mode, GLint first, GLsizei count, GLsizei primcount);
void (APIENTRYP glGetIntegerv)(GLenum pname, GLint *params);
GLenum (APIENTRYP glGetError)(void);
const GLubyte* (APIENTRYP glGetString)(GLenum name);

GLuint (APIENTRYP glCreateShader)(GLenum type);
void (APIENTRYP glShaderSource)(GLuint shader, GLsizei count, const GLchar *const *string, const GLint *length);
void (APIENTRYP glCompileShader)(GLuint shader);
void (APIENTRYP glGetShaderiv)(GLuint shader, GLenum pname, GLint *params);
void (APIENTRYP glGetShaderInfoLog)(GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
void (APIENTRYP glDeleteShader)(GLuint shader);

GLuint (APIENTRYP glCreateProgram)(void);
void (APIENTRYP glAttachShader)(GLuint program, GLuint shader);
void (APIENTRYP glLinkProgram)(GLuint program);
void (APIENTRYP glGetProgramiv)(GLuint program, GLenum pname, GLint *params);
void (APIENTRYP glGetProgramInfoLog)(GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
void (APIENTRYP glUseProgram)(GLuint program);
void (APIENTRYP glDeleteProgram)(GLuint program);

GLint (APIENTRYP glGetUniformLocation)(GLuint program, const GLchar *name);
void (APIENTRYP glUniform1i)(GLint location, GLint v0);
void (APIENTRYP glUniform1f)(GLint location, GLfloat v0);
void (APIENTRYP glUniform3fv)(GLint location, GLsizei count, const GLfloat *value);
void (APIENTRYP glUniformMatrix4fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);

void (APIENTRYP glGenBuffers)(GLsizei n, GLuint *buffers);
void (APIENTRYP glBindBuffer)(GLenum target, GLuint buffer);
void (APIENTRYP glBufferData)(GLenum target, GLsizeiptr size, const GLvoid *data, GLenum usage);
void (APIENTRYP glBufferSubData)(GLenum target, GLintptr offset, GLsizeiptr size, const GLvoid *data);
void (APIENTRYP glDeleteBuffers)(GLsizei n, const GLuint *buffers);

void (APIENTRYP glGenVertexArrays)(GLsizei n, GLuint *arrays);
void (APIENTRYP glBindVertexArray)(GLuint array);
void (APIENTRYP glDeleteVertexArrays)(GLsizei n, const GLuint *arrays);
void (APIENTRYP glVertexAttribPointer)(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *pointer);
void (APIENTRYP glEnableVertexAttribArray)(GLuint index);
void (APIENTRYP glDisableVertexAttribArray)(GLuint index);
void (APIENTRYP glVertexAttribDivisor)(GLuint index, GLuint divisor);

void (APIENTRYP glGenTextures)(GLsizei n, GLuint *textures);
void (APIENTRYP glBindTexture)(GLenum target, GLuint texture);
void (APIENTRYP glTexImage2D)(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
void (APIENTRYP glTexParameteri)(GLenum target, GLenum pname, GLint param);
void (APIENTRYP glActiveTexture)(GLenum texture);
void (APIENTRYP glDeleteTextures)(GLsizei n, const GLuint *textures);

int gladLoadGLLoader(GLADloadproc load) {
    glClearColor = (void (APIENTRYP)(GLfloat, GLfloat, GLfloat, GLfloat))load("glClearColor");
    glClear = (void (APIENTRYP)(GLbitfield))load("glClear");
    glViewport = (void (APIENTRYP)(GLint, GLint, GLsizei, GLsizei))load("glViewport");
    glEnable = (void (APIENTRYP)(GLenum))load("glEnable");
    glDisable = (void (APIENTRYP)(GLenum))load("glDisable");
    glBlendFunc = (void (APIENTRYP)(GLenum, GLenum))load("glBlendFunc");
    glDepthFunc = (void (APIENTRYP)(GLenum))load("glDepthFunc");
    glDrawArrays = (void (APIENTRYP)(GLenum, GLint, GLsizei))load("glDrawArrays");
    glDrawElements = (void (APIENTRYP)(GLenum, GLsizei, GLenum, const GLvoid*))load("glDrawElements");
    glDrawArraysInstanced = (void (APIENTRYP)(GLenum, GLint, GLsizei, GLsizei))load("glDrawArraysInstanced");
    glGetIntegerv = (void (APIENTRYP)(GLenum, GLint*))load("glGetIntegerv");
    glGetError = (GLenum (APIENTRYP)(void))load("glGetError");
    glGetString = (const GLubyte* (APIENTRYP)(GLenum))load("glGetString");
    
    glCreateShader = (GLuint (APIENTRYP)(GLenum))load("glCreateShader");
    glShaderSource = (void (APIENTRYP)(GLuint, GLsizei, const GLchar *const *, const GLint*))load("glShaderSource");
    glCompileShader = (void (APIENTRYP)(GLuint))load("glCompileShader");
    glGetShaderiv = (void (APIENTRYP)(GLuint, GLenum, GLint*))load("glGetShaderiv");
    glGetShaderInfoLog = (void (APIENTRYP)(GLuint, GLsizei, GLsizei*, GLchar*))load("glGetShaderInfoLog");
    glDeleteShader = (void (APIENTRYP)(GLuint))load("glDeleteShader");
    
    glCreateProgram = (GLuint (APIENTRYP)(void))load("glCreateProgram");
    glAttachShader = (void (APIENTRYP)(GLuint, GLuint))load("glAttachShader");
    glLinkProgram = (void (APIENTRYP)(GLuint))load("glLinkProgram");
    glGetProgramiv = (void (APIENTRYP)(GLuint, GLenum, GLint*))load("glGetProgramiv");
    glGetProgramInfoLog = (void (APIENTRYP)(GLuint, GLsizei, GLsizei*, GLchar*))load("glGetProgramInfoLog");
    glUseProgram = (void (APIENTRYP)(GLuint))load("glUseProgram");
    glDeleteProgram = (void (APIENTRYP)(GLuint))load("glDeleteProgram");
    
    glGetUniformLocation = (GLint (APIENTRYP)(GLuint, const GLchar*))load("glGetUniformLocation");
    glUniform1i = (void (APIENTRYP)(GLint, GLint))load("glUniform1i");
    glUniform1f = (void (APIENTRYP)(GLint, GLfloat))load("glUniform1f");
    glUniform3fv = (void (APIENTRYP)(GLint, GLsizei, const GLfloat*))load("glUniform3fv");
    glUniformMatrix4fv = (void (APIENTRYP)(GLint, GLsizei, GLboolean, const GLfloat*))load("glUniformMatrix4fv");
    
    glGenBuffers = (void (APIENTRYP)(GLsizei, GLuint*))load("glGenBuffers");
    glBindBuffer = (void (APIENTRYP)(GLenum, GLuint))load("glBindBuffer");
    glBufferData = (void (APIENTRYP)(GLenum, GLsizeiptr, const GLvoid*, GLenum))load("glBufferData");
    glBufferSubData = (void (APIENTRYP)(GLenum, GLintptr, GLsizeiptr, const GLvoid*))load("glBufferSubData");
    glDeleteBuffers = (void (APIENTRYP)(GLsizei, const GLuint*))load("glDeleteBuffers");
    
    glGenVertexArrays = (void (APIENTRYP)(GLsizei, GLuint*))load("glGenVertexArrays");
    glBindVertexArray = (void (APIENTRYP)(GLuint))load("glBindVertexArray");
    glDeleteVertexArrays = (void (APIENTRYP)(GLsizei, const GLuint*))load("glDeleteVertexArrays");
    glVertexAttribPointer = (void (APIENTRYP)(GLuint, GLint, GLenum, GLboolean, GLsizei, const GLvoid*))load("glVertexAttribPointer");
    glEnableVertexAttribArray = (void (APIENTRYP)(GLuint))load("glEnableVertexAttribArray");
    glDisableVertexAttribArray = (void (APIENTRYP)(GLuint))load("glDisableVertexAttribArray");
    glVertexAttribDivisor = (void (APIENTRYP)(GLuint, GLuint))load("glVertexAttribDivisor");
    
    glGenTextures = (void (APIENTRYP)(GLsizei, GLuint*))load("glGenTextures");
    glBindTexture = (void (APIENTRYP)(GLenum, GLuint))load("glBindTexture");
    glTexImage2D = (void (APIENTRYP)(GLenum, GLint, GLint, GLsizei, GLsizei, GLint, GLenum, GLenum, const GLvoid*))load("glTexImage2D");
    glTexParameteri = (void (APIENTRYP)(GLenum, GLenum, GLint))load("glTexParameteri");
    glActiveTexture = (void (APIENTRYP)(GLenum))load("glActiveTexture");
    glDeleteTextures = (void (APIENTRYP)(GLsizei, const GLuint*))load("glDeleteTextures");
    
    GLVersion.major = 3;
    GLVersion.minor = 3;
    
    return 1;
}

int gladLoadGL(void) {
#ifdef _WIN32
    libGL = LoadLibraryA("opengl32.dll");
    if(libGL != NULL) {
        return gladLoadGLLoader((GLADloadproc)get_proc);
    }
    return 0;
#else
    libGL = dlopen("libGL.so.1", RTLD_NOW | RTLD_GLOBAL);
    if(libGL != NULL) {
        return gladLoadGLLoader((GLADloadproc)get_proc);
    }
    return 0;
#endif
}