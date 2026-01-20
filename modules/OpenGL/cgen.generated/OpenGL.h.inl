/* Generated code */

#ifndef OPENGL_FUNCTIONS_H
#define OPENGL_FUNCTIONS_H

function b32 _os_opengl_load_functions(); /* Loads all opengl functions in the on the code gen Table */

typedef void (*PFNglProgramUniform1iPROC)(GLuint program, GLint location, GLint v0);
global PFNglProgramUniform1iPROC __glProgramUniform1i = NULL;
function inline void
glProgramUniform1i (GLuint program, GLint location, GLint v0)
{
  __glProgramUniform1i (program, location, v0);
}

typedef void (*PFNglProgramUniform1fPROC)(GLuint program, GLint location, GLfloat v0);
global PFNglProgramUniform1fPROC __glProgramUniform1f = NULL;
function inline void
glProgramUniform1f (GLuint program, GLint location, GLfloat v0)
{
  __glProgramUniform1f (program, location, v0);
}

typedef void (*PFNglProgramUniform2fPROC)(GLuint program, GLint location, GLfloat v0, GLfloat v1);
global PFNglProgramUniform2fPROC __glProgramUniform2f = NULL;
function inline void
glProgramUniform2f (GLuint program, GLint location, GLfloat v0, GLfloat v1)
{
  __glProgramUniform2f (program, location, v0, v1);
}

typedef void (*PFNglProgramUniform3fPROC)(GLuint program, GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
global PFNglProgramUniform3fPROC __glProgramUniform3f = NULL;
function inline void
glProgramUniform3f (GLuint program, GLint location, GLfloat v0, GLfloat v1, GLfloat v2)
{
  __glProgramUniform3f (program, location, v0, v1, v2);
}

typedef void (*PFNglProgramUniform4fPROC)(GLuint program, GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
global PFNglProgramUniform4fPROC __glProgramUniform4f = NULL;
function inline void
glProgramUniform4f (GLuint program, GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3)
{
  __glProgramUniform4f (program, location, v0, v1, v2, v3);
}

typedef void (*PFNglProgramUniformMatrix4fvPROC)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
global PFNglProgramUniformMatrix4fvPROC __glProgramUniformMatrix4fv = NULL;
function inline void
glProgramUniformMatrix4fv (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)
{
  __glProgramUniformMatrix4fv (program, location, count, transpose, value);
}

typedef void (*PFNglProgramUniformMatrix2fvPROC)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
global PFNglProgramUniformMatrix2fvPROC __glProgramUniformMatrix2fv = NULL;
function inline void
glProgramUniformMatrix2fv (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)
{
  __glProgramUniformMatrix2fv (program, location, count, transpose, value);
}

typedef void (*PFNglAttachShaderPROC)(GLuint program, GLuint shader);
global PFNglAttachShaderPROC __glAttachShader = NULL;
function inline void
glAttachShader (GLuint program, GLuint shader)
{
  __glAttachShader (program, shader);
}

typedef void (*PFNglLinkProgramPROC)(GLuint program);
global PFNglLinkProgramPROC __glLinkProgram = NULL;
function inline void
glLinkProgram (GLuint program)
{
  __glLinkProgram (program);
}

typedef void (*PFNglDeleteShaderPROC)(GLuint shader);
global PFNglDeleteShaderPROC __glDeleteShader = NULL;
function inline void
glDeleteShader (GLuint shader)
{
  __glDeleteShader (shader);
}

typedef void (*PFNglDeleteProgramPROC)(GLuint program);
global PFNglDeleteProgramPROC __glDeleteProgram = NULL;
function inline void
glDeleteProgram (GLuint program)
{
  __glDeleteProgram (program);
}

typedef void (*PFNglGetProgramivPROC)(GLuint program, GLenum pname, GLint *params);
global PFNglGetProgramivPROC __glGetProgramiv = NULL;
function inline void
glGetProgramiv (GLuint program, GLenum pname, GLint *params)
{
  __glGetProgramiv (program, pname, params);
}

typedef void (*PFNglGetProgramInfoLogPROC)(GLuint program, GLsizei bufSize, GLsizei *length, char *infoLog);
global PFNglGetProgramInfoLogPROC __glGetProgramInfoLog = NULL;
function inline void
glGetProgramInfoLog (GLuint program, GLsizei bufSize, GLsizei *length, char *infoLog)
{
  __glGetProgramInfoLog (program, bufSize, length, infoLog);
}

typedef void (*PFNglGenProgramPipelinesPROC)(GLsizei n, GLuint *pipelines);
global PFNglGenProgramPipelinesPROC __glGenProgramPipelines = NULL;
function inline void
glGenProgramPipelines (GLsizei n, GLuint *pipelines)
{
  __glGenProgramPipelines (n, pipelines);
}

typedef void (*PFNglBindProgramPipelinePROC)(GLuint pipeline);
global PFNglBindProgramPipelinePROC __glBindProgramPipeline = NULL;
function inline void
glBindProgramPipeline (GLuint pipeline)
{
  __glBindProgramPipeline (pipeline);
}

typedef void (*PFNglUseProgramStagesPROC)(GLuint pipeline, GLbitfield stages, GLuint program);
global PFNglUseProgramStagesPROC __glUseProgramStages = NULL;
function inline void
glUseProgramStages (GLuint pipeline, GLbitfield stages, GLuint program)
{
  __glUseProgramStages (pipeline, stages, program);
}

typedef void (*PFNglDeleteProgramPipelinesPROC)(GLsizei n, const GLuint *pipelines);
global PFNglDeleteProgramPipelinesPROC __glDeleteProgramPipelines = NULL;
function inline void
glDeleteProgramPipelines (GLsizei n, const GLuint *pipelines)
{
  __glDeleteProgramPipelines (n, pipelines);
}

typedef void (*PFNglCreateBuffersPROC)(GLsizei n, GLuint *buffers);
global PFNglCreateBuffersPROC __glCreateBuffers = NULL;
function inline void
glCreateBuffers (GLsizei n, GLuint *buffers)
{
  __glCreateBuffers (n, buffers);
}

typedef void (*PFNglNamedBufferDataPROC)(GLuint buffer, GLsizeiptr size, const void *data, GLenum usage);
global PFNglNamedBufferDataPROC __glNamedBufferData = NULL;
function inline void
glNamedBufferData (GLuint buffer, GLsizeiptr size, const void *data, GLenum usage)
{
  __glNamedBufferData (buffer, size, data, usage);
}

typedef void (*PFNglNamedBufferSubDataPROC)(GLuint buffer, GLintptr offset, GLsizeiptr size, const void *data);
global PFNglNamedBufferSubDataPROC __glNamedBufferSubData = NULL;
function inline void
glNamedBufferSubData (GLuint buffer, GLintptr offset, GLsizeiptr size, const void *data)
{
  __glNamedBufferSubData (buffer, offset, size, data);
}

typedef void (*PFNglDeleteBuffersPROC)(GLsizei n, const GLuint *buffers);
global PFNglDeleteBuffersPROC __glDeleteBuffers = NULL;
function inline void
glDeleteBuffers (GLsizei n, const GLuint *buffers)
{
  __glDeleteBuffers (n, buffers);
}

typedef void (*PFNglBindBufferPROC)(GLenum target, GLuint buffer);
global PFNglBindBufferPROC __glBindBuffer = NULL;
function inline void
glBindBuffer (GLenum target, GLuint buffer)
{
  __glBindBuffer (target, buffer);
}

typedef void (*PFNglNamedBufferStoragePROC)(GLuint buffer, GLsizeiptr size, const void *data, GLbitfield flags);
global PFNglNamedBufferStoragePROC __glNamedBufferStorage = NULL;
function inline void
glNamedBufferStorage (GLuint buffer, GLsizeiptr size, const void *data, GLbitfield flags)
{
  __glNamedBufferStorage (buffer, size, data, flags);
}

typedef void (*PFNglCreateProgramPipelinesPROC)(GLsizei n, GLuint *pipelines);
global PFNglCreateProgramPipelinesPROC __glCreateProgramPipelines = NULL;
function inline void
glCreateProgramPipelines (GLsizei n, GLuint *pipelines)
{
  __glCreateProgramPipelines (n, pipelines);
}

typedef void (*PFNglCreateVertexArraysPROC)(GLsizei n, GLuint *arrays);
global PFNglCreateVertexArraysPROC __glCreateVertexArrays = NULL;
function inline void
glCreateVertexArrays (GLsizei n, GLuint *arrays)
{
  __glCreateVertexArrays (n, arrays);
}

typedef void (*PFNglVertexArrayVertexBufferPROC)(GLuint vaobj, GLuint bindingindex, GLuint buffer, GLintptr offset, GLsizei stride);
global PFNglVertexArrayVertexBufferPROC __glVertexArrayVertexBuffer = NULL;
function inline void
glVertexArrayVertexBuffer (GLuint vaobj, GLuint bindingindex, GLuint buffer, GLintptr offset, GLsizei stride)
{
  __glVertexArrayVertexBuffer (vaobj, bindingindex, buffer, offset, stride);
}

typedef void (*PFNglEnableVertexArrayAttribPROC)(GLuint vaobj, GLuint index);
global PFNglEnableVertexArrayAttribPROC __glEnableVertexArrayAttrib = NULL;
function inline void
glEnableVertexArrayAttrib (GLuint vaobj, GLuint index)
{
  __glEnableVertexArrayAttrib (vaobj, index);
}

typedef void (*PFNglVertexArrayAttribFormatPROC)(GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLboolean normalized, GLuint relativeoffset);
global PFNglVertexArrayAttribFormatPROC __glVertexArrayAttribFormat = NULL;
function inline void
glVertexArrayAttribFormat (GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLboolean normalized, GLuint relativeoffset)
{
  __glVertexArrayAttribFormat (vaobj, attribindex, size, type, normalized, relativeoffset);
}

typedef void (*PFNglVertexArrayAttribBindingPROC)(GLuint vaobj, GLuint attribindex, GLuint bindingindex);
global PFNglVertexArrayAttribBindingPROC __glVertexArrayAttribBinding = NULL;
function inline void
glVertexArrayAttribBinding (GLuint vaobj, GLuint attribindex, GLuint bindingindex)
{
  __glVertexArrayAttribBinding (vaobj, attribindex, bindingindex);
}

typedef void (*PFNglBindVertexArrayPROC)(GLuint array);
global PFNglBindVertexArrayPROC __glBindVertexArray = NULL;
function inline void
glBindVertexArray (GLuint array)
{
  __glBindVertexArray (array);
}

typedef void (*PFNglDeleteVertexArraysPROC)(GLsizei n, const GLuint *arrays);
global PFNglDeleteVertexArraysPROC __glDeleteVertexArrays = NULL;
function inline void
glDeleteVertexArrays (GLsizei n, const GLuint *arrays)
{
  __glDeleteVertexArrays (n, arrays);
}

typedef void (*PFNglVertexArrayBindingDivisorPROC)(GLuint vaobj, GLuint bindingindex, GLuint divisor);
global PFNglVertexArrayBindingDivisorPROC __glVertexArrayBindingDivisor = NULL;
function inline void
glVertexArrayBindingDivisor (GLuint vaobj, GLuint bindingindex, GLuint divisor)
{
  __glVertexArrayBindingDivisor (vaobj, bindingindex, divisor);
}

typedef void (*PFNglCreateTexturesPROC)(GLenum target, GLsizei n, GLuint *textures);
global PFNglCreateTexturesPROC __glCreateTextures = NULL;
function inline void
glCreateTextures (GLenum target, GLsizei n, GLuint *textures)
{
  __glCreateTextures (target, n, textures);
}

typedef void (*PFNglTextureStorage2DPROC)(GLuint texture, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height);
global PFNglTextureStorage2DPROC __glTextureStorage2D = NULL;
function inline void
glTextureStorage2D (GLuint texture, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height)
{
  __glTextureStorage2D (texture, levels, internalformat, width, height);
}

typedef void (*PFNglTextureSubImage2DPROC)(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *pixels);
global PFNglTextureSubImage2DPROC __glTextureSubImage2D = NULL;
function inline void
glTextureSubImage2D (GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *pixels)
{
  __glTextureSubImage2D (texture, level, xoffset, yoffset, width, height, format, type, pixels);
}

typedef void (*PFNglTextureParameteriPROC)(GLuint texture, GLenum pname, GLint param);
global PFNglTextureParameteriPROC __glTextureParameteri = NULL;
function inline void
glTextureParameteri (GLuint texture, GLenum pname, GLint param)
{
  __glTextureParameteri (texture, pname, param);
}

typedef void (*PFNglBindTextureUnitPROC)(GLuint unit, GLuint texture);
global PFNglBindTextureUnitPROC __glBindTextureUnit = NULL;
function inline void
glBindTextureUnit (GLuint unit, GLuint texture)
{
  __glBindTextureUnit (unit, texture);
}

typedef void (*PFNglDeleteTexturesPROC)(GLsizei n, const GLuint *textures);
global PFNglDeleteTexturesPROC __glDeleteTextures = NULL;
function inline void
glDeleteTextures (GLsizei n, const GLuint *textures)
{
  __glDeleteTextures (n, textures);
}

typedef void (*PFNglCreateFramebuffersPROC)(GLsizei n, GLuint *framebuffers);
global PFNglCreateFramebuffersPROC __glCreateFramebuffers = NULL;
function inline void
glCreateFramebuffers (GLsizei n, GLuint *framebuffers)
{
  __glCreateFramebuffers (n, framebuffers);
}

typedef void (*PFNglNamedFramebufferTexturePROC)(GLuint framebuffer, GLenum attachment, GLuint texture, GLint level);
global PFNglNamedFramebufferTexturePROC __glNamedFramebufferTexture = NULL;
function inline void
glNamedFramebufferTexture (GLuint framebuffer, GLenum attachment, GLuint texture, GLint level)
{
  __glNamedFramebufferTexture (framebuffer, attachment, texture, level);
}

typedef void (*PFNglNamedFramebufferRenderbufferPROC)(GLuint framebuffer, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
global PFNglNamedFramebufferRenderbufferPROC __glNamedFramebufferRenderbuffer = NULL;
function inline void
glNamedFramebufferRenderbuffer (GLuint framebuffer, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer)
{
  __glNamedFramebufferRenderbuffer (framebuffer, attachment, renderbuffertarget, renderbuffer);
}

typedef void (*PFNglDeleteFramebuffersPROC)(GLsizei n, const GLuint *framebuffers);
global PFNglDeleteFramebuffersPROC __glDeleteFramebuffers = NULL;
function inline void
glDeleteFramebuffers (GLsizei n, const GLuint *framebuffers)
{
  __glDeleteFramebuffers (n, framebuffers);
}

typedef void (*PFNglBindFramebufferPROC)(GLenum target, GLuint framebuffer);
global PFNglBindFramebufferPROC __glBindFramebuffer = NULL;
function inline void
glBindFramebuffer (GLenum target, GLuint framebuffer)
{
  __glBindFramebuffer (target, framebuffer);
}

typedef void (*PFNglDrawArraysPROC)(GLenum mode, GLint first, GLsizei count);
global PFNglDrawArraysPROC __glDrawArrays = NULL;
function inline void
glDrawArrays (GLenum mode, GLint first, GLsizei count)
{
  __glDrawArrays (mode, first, count);
}

typedef void (*PFNglDrawElementsPROC)(GLenum mode, GLsizei count, GLenum type, const void *indices);
global PFNglDrawElementsPROC __glDrawElements = NULL;
function inline void
glDrawElements (GLenum mode, GLsizei count, GLenum type, const void *indices)
{
  __glDrawElements (mode, count, type, indices);
}

typedef void (*PFNglDrawArraysInstancedPROC)(GLenum mode, GLint first, GLsizei count, GLsizei instancecount);
global PFNglDrawArraysInstancedPROC __glDrawArraysInstanced = NULL;
function inline void
glDrawArraysInstanced (GLenum mode, GLint first, GLsizei count, GLsizei instancecount)
{
  __glDrawArraysInstanced (mode, first, count, instancecount);
}

typedef void (*PFNglDrawElementsInstancedPROC)(GLenum mode, GLsizei count, GLenum type, const void *indices, GLsizei instancecount);
global PFNglDrawElementsInstancedPROC __glDrawElementsInstanced = NULL;
function inline void
glDrawElementsInstanced (GLenum mode, GLsizei count, GLenum type, const void *indices, GLsizei instancecount)
{
  __glDrawElementsInstanced (mode, count, type, indices, instancecount);
}

typedef void (*PFNglClearColorPROC)(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
global PFNglClearColorPROC __glClearColor = NULL;
function inline void
glClearColor (GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
{
  __glClearColor (red, green, blue, alpha);
}

typedef void (*PFNglDebugMessageCallbackPROC)(GLDEBUGPROC callback, const void *userParam);
global PFNglDebugMessageCallbackPROC __glDebugMessageCallback = NULL;
function inline void
glDebugMessageCallback (GLDEBUGPROC callback, const void *userParam)
{
  __glDebugMessageCallback (callback, userParam);
}

typedef void (*PFNglEnablePROC)(GLenum cap);
global PFNglEnablePROC __glEnable = NULL;
function inline void
glEnable (GLenum cap)
{
  __glEnable (cap);
}

typedef void (*PFNglGetIntegervPROC)(GLenum pname, GLint *data);
global PFNglGetIntegervPROC __glGetIntegerv = NULL;
function inline void
glGetIntegerv (GLenum pname, GLint *data)
{
  __glGetIntegerv (pname, data);
}

typedef void (*PFNglVertexArrayAttribIFormatPROC)(GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset);
global PFNglVertexArrayAttribIFormatPROC __glVertexArrayAttribIFormat = NULL;
function inline void
glVertexArrayAttribIFormat (GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset)
{
  __glVertexArrayAttribIFormat (vaobj, attribindex, size, type, relativeoffset);
}

typedef void (*PFNglUniform1ivPROC)(GLint location, GLsizei count, const GLint *value);
global PFNglUniform1ivPROC __glUniform1iv = NULL;
function inline void
glUniform1iv (GLint location, GLsizei count, const GLint *value)
{
  __glUniform1iv (location, count, value);
}

typedef void (*PFNglUseProgramPROC)(GLuint program);
global PFNglUseProgramPROC __glUseProgram = NULL;
function inline void
glUseProgram (GLuint program)
{
  __glUseProgram (program);
}

typedef void (*PFNglProgramUniform1ivPROC)(GLuint program, GLint location, GLsizei count, const GLint *value);
global PFNglProgramUniform1ivPROC __glProgramUniform1iv = NULL;
function inline void
glProgramUniform1iv (GLuint program, GLint location, GLsizei count, const GLint *value)
{
  __glProgramUniform1iv (program, location, count, value);
}

typedef void (*PFNglPolygonModePROC)(GLenum face, GLenum mode);
global PFNglPolygonModePROC __glPolygonMode = NULL;
function inline void
glPolygonMode (GLenum face, GLenum mode)
{
  __glPolygonMode (face, mode);
}

typedef void (*PFNglDepthRangefPROC)(GLfloat n, GLfloat f);
global PFNglDepthRangefPROC __glDepthRangef = NULL;
function inline void
glDepthRangef (GLfloat n, GLfloat f)
{
  __glDepthRangef (n, f);
}

typedef void (*PFNglLineWidthPROC)(GLfloat width);
global PFNglLineWidthPROC __glLineWidth = NULL;
function inline void
glLineWidth (GLfloat width)
{
  __glLineWidth (width);
}

typedef void (*PFNglVertexArrayElementBufferPROC)(GLuint vaobj, GLuint buffer);
global PFNglVertexArrayElementBufferPROC __glVertexArrayElementBuffer = NULL;
function inline void
glVertexArrayElementBuffer (GLuint vaobj, GLuint buffer)
{
  __glVertexArrayElementBuffer (vaobj, buffer);
}

typedef void (*PFNglProgramUniformMatrix3fvPROC)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
global PFNglProgramUniformMatrix3fvPROC __glProgramUniformMatrix3fv = NULL;
function inline void
glProgramUniformMatrix3fv (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)
{
  __glProgramUniformMatrix3fv (program, location, count, transpose, value);
}

typedef void (*PFNglViewportPROC)(GLint x, GLint y, GLsizei width, GLsizei height);
global PFNglViewportPROC __glViewport = NULL;
function inline void
glViewport (GLint x, GLint y, GLsizei width, GLsizei height)
{
  __glViewport (x, y, width, height);
}

typedef void (*PFNglBlendFuncPROC)(GLenum sfactor, GLenum dfactor);
global PFNglBlendFuncPROC __glBlendFunc = NULL;
function inline void
glBlendFunc (GLenum sfactor, GLenum dfactor)
{
  __glBlendFunc (sfactor, dfactor);
}

typedef void (*PFNglClearPROC)(GLbitfield mask);
global PFNglClearPROC __glClear = NULL;
function inline void
glClear (GLbitfield mask)
{
  __glClear (mask);
}

typedef GLuint (*PFNglCreateShaderProgramvPROC)(GLenum type, GLsizei count, const char *const* strings);
global PFNglCreateShaderProgramvPROC __glCreateShaderProgramv = NULL;
function inline GLuint
glCreateShaderProgramv (GLenum type, GLsizei count, const char *const* strings)
{
  return __glCreateShaderProgramv (type, count, strings);
}

typedef GLint (*PFNglGetUniformLocationPROC)(GLuint program, const GLchar *name);
global PFNglGetUniformLocationPROC __glGetUniformLocation = NULL;
function inline GLint
glGetUniformLocation (GLuint program, const GLchar *name)
{
  return __glGetUniformLocation (program, name);
}

typedef GLuint (*PFNglCreateProgramPROC)(void);
global PFNglCreateProgramPROC __glCreateProgram = NULL;
function inline GLuint
glCreateProgram (void)
{
  return __glCreateProgram ();
}

typedef GLenum (*PFNglCheckNamedFramebufferStatusPROC)(GLuint framebuffer, GLenum target);
global PFNglCheckNamedFramebufferStatusPROC __glCheckNamedFramebufferStatus = NULL;
function inline GLenum
glCheckNamedFramebufferStatus (GLuint framebuffer, GLenum target)
{
  return __glCheckNamedFramebufferStatus (framebuffer, target);
}

typedef const GLubyte * (*PFNglGetStringPROC)(GLenum name);
global PFNglGetStringPROC __glGetString = NULL;
function inline const GLubyte *
glGetString (GLenum name)
{
  return __glGetString (name);
}

typedef void* (*PFNglMapNamedBufferRangePROC)(GLuint buffer, GLintptr offset, GLsizeiptr length, GLbitfield access);
global PFNglMapNamedBufferRangePROC __glMapNamedBufferRange = NULL;
function inline void*
glMapNamedBufferRange (GLuint buffer, GLintptr offset, GLsizeiptr length, GLbitfield access)
{
  return __glMapNamedBufferRange (buffer, offset, length, access);
}

typedef GLenum (*PFNglGetErrorPROC)(void);
global PFNglGetErrorPROC __glGetError = NULL;
function inline GLenum
glGetError (void)
{
  return __glGetError ();
}

#endif // OPENGL_FUNCTIONS_H