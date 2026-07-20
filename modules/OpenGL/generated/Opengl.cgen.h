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
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglProgramUniform1fPROC)(GLuint program, GLint location, GLfloat v0);
global PFNglProgramUniform1fPROC __glProgramUniform1f = NULL;
function inline void
glProgramUniform1f (GLuint program, GLint location, GLfloat v0)
{
  __glProgramUniform1f (program, location, v0);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglProgramUniform2fPROC)(GLuint program, GLint location, GLfloat v0, GLfloat v1);
global PFNglProgramUniform2fPROC __glProgramUniform2f = NULL;
function inline void
glProgramUniform2f (GLuint program, GLint location, GLfloat v0, GLfloat v1)
{
  __glProgramUniform2f (program, location, v0, v1);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglProgramUniform3fPROC)(GLuint program, GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
global PFNglProgramUniform3fPROC __glProgramUniform3f = NULL;
function inline void
glProgramUniform3f (GLuint program, GLint location, GLfloat v0, GLfloat v1, GLfloat v2)
{
  __glProgramUniform3f (program, location, v0, v1, v2);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglProgramUniform4fPROC)(GLuint program, GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
global PFNglProgramUniform4fPROC __glProgramUniform4f = NULL;
function inline void
glProgramUniform4f (GLuint program, GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3)
{
  __glProgramUniform4f (program, location, v0, v1, v2, v3);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglProgramUniform3fvPROC)(GLuint program, GLint location, GLsizei count, const GLfloat *value);
global PFNglProgramUniform3fvPROC __glProgramUniform3fv = NULL;
function inline void
glProgramUniform3fv (GLuint program, GLint location, GLsizei count, const GLfloat *value)
{
  __glProgramUniform3fv (program, location, count, value);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglProgramUniformMatrix4fvPROC)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
global PFNglProgramUniformMatrix4fvPROC __glProgramUniformMatrix4fv = NULL;
function inline void
glProgramUniformMatrix4fv (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)
{
  __glProgramUniformMatrix4fv (program, location, count, transpose, value);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglProgramUniformMatrix2fvPROC)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
global PFNglProgramUniformMatrix2fvPROC __glProgramUniformMatrix2fv = NULL;
function inline void
glProgramUniformMatrix2fv (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)
{
  __glProgramUniformMatrix2fv (program, location, count, transpose, value);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglAttachShaderPROC)(GLuint program, GLuint shader);
global PFNglAttachShaderPROC __glAttachShader = NULL;
function inline void
glAttachShader (GLuint program, GLuint shader)
{
  __glAttachShader (program, shader);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglLinkProgramPROC)(GLuint program);
global PFNglLinkProgramPROC __glLinkProgram = NULL;
function inline void
glLinkProgram (GLuint program)
{
  __glLinkProgram (program);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglDeleteShaderPROC)(GLuint shader);
global PFNglDeleteShaderPROC __glDeleteShader = NULL;
function inline void
glDeleteShader (GLuint shader)
{
  __glDeleteShader (shader);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglDeleteProgramPROC)(GLuint program);
global PFNglDeleteProgramPROC __glDeleteProgram = NULL;
function inline void
glDeleteProgram (GLuint program)
{
  __glDeleteProgram (program);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglGetProgramivPROC)(GLuint program, GLenum pname, GLint *params);
global PFNglGetProgramivPROC __glGetProgramiv = NULL;
function inline void
glGetProgramiv (GLuint program, GLenum pname, GLint *params)
{
  __glGetProgramiv (program, pname, params);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglGetProgramInfoLogPROC)(GLuint program, GLsizei bufSize, GLsizei *length, char *infoLog);
global PFNglGetProgramInfoLogPROC __glGetProgramInfoLog = NULL;
function inline void
glGetProgramInfoLog (GLuint program, GLsizei bufSize, GLsizei *length, char *infoLog)
{
  __glGetProgramInfoLog (program, bufSize, length, infoLog);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglGenProgramPipelinesPROC)(GLsizei n, GLuint *pipelines);
global PFNglGenProgramPipelinesPROC __glGenProgramPipelines = NULL;
function inline void
glGenProgramPipelines (GLsizei n, GLuint *pipelines)
{
  __glGenProgramPipelines (n, pipelines);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglBindProgramPipelinePROC)(GLuint pipeline);
global PFNglBindProgramPipelinePROC __glBindProgramPipeline = NULL;
function inline void
glBindProgramPipeline (GLuint pipeline)
{
  __glBindProgramPipeline (pipeline);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglUseProgramStagesPROC)(GLuint pipeline, GLbitfield stages, GLuint program);
global PFNglUseProgramStagesPROC __glUseProgramStages = NULL;
function inline void
glUseProgramStages (GLuint pipeline, GLbitfield stages, GLuint program)
{
  __glUseProgramStages (pipeline, stages, program);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglDeleteProgramPipelinesPROC)(GLsizei n, const GLuint *pipelines);
global PFNglDeleteProgramPipelinesPROC __glDeleteProgramPipelines = NULL;
function inline void
glDeleteProgramPipelines (GLsizei n, const GLuint *pipelines)
{
  __glDeleteProgramPipelines (n, pipelines);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglCreateBuffersPROC)(GLsizei n, GLuint *buffers);
global PFNglCreateBuffersPROC __glCreateBuffers = NULL;
function inline void
glCreateBuffers (GLsizei n, GLuint *buffers)
{
  __glCreateBuffers (n, buffers);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglNamedBufferDataPROC)(GLuint buffer, GLsizeiptr size, const void *data, GLenum usage);
global PFNglNamedBufferDataPROC __glNamedBufferData = NULL;
function inline void
glNamedBufferData (GLuint buffer, GLsizeiptr size, const void *data, GLenum usage)
{
  __glNamedBufferData (buffer, size, data, usage);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglNamedBufferSubDataPROC)(GLuint buffer, GLintptr offset, GLsizeiptr size, const void *data);
global PFNglNamedBufferSubDataPROC __glNamedBufferSubData = NULL;
function inline void
glNamedBufferSubData (GLuint buffer, GLintptr offset, GLsizeiptr size, const void *data)
{
  __glNamedBufferSubData (buffer, offset, size, data);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglDeleteBuffersPROC)(GLsizei n, const GLuint *buffers);
global PFNglDeleteBuffersPROC __glDeleteBuffers = NULL;
function inline void
glDeleteBuffers (GLsizei n, const GLuint *buffers)
{
  __glDeleteBuffers (n, buffers);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglBindBufferPROC)(GLenum target, GLuint buffer);
global PFNglBindBufferPROC __glBindBuffer = NULL;
function inline void
glBindBuffer (GLenum target, GLuint buffer)
{
  __glBindBuffer (target, buffer);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglNamedBufferStoragePROC)(GLuint buffer, GLsizeiptr size, const void *data, GLbitfield flags);
global PFNglNamedBufferStoragePROC __glNamedBufferStorage = NULL;
function inline void
glNamedBufferStorage (GLuint buffer, GLsizeiptr size, const void *data, GLbitfield flags)
{
  __glNamedBufferStorage (buffer, size, data, flags);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglCreateProgramPipelinesPROC)(GLsizei n, GLuint *pipelines);
global PFNglCreateProgramPipelinesPROC __glCreateProgramPipelines = NULL;
function inline void
glCreateProgramPipelines (GLsizei n, GLuint *pipelines)
{
  __glCreateProgramPipelines (n, pipelines);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglCreateVertexArraysPROC)(GLsizei n, GLuint *arrays);
global PFNglCreateVertexArraysPROC __glCreateVertexArrays = NULL;
function inline void
glCreateVertexArrays (GLsizei n, GLuint *arrays)
{
  __glCreateVertexArrays (n, arrays);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglVertexArrayVertexBufferPROC)(GLuint vaobj, GLuint bindingindex, GLuint buffer, GLintptr offset, GLsizei stride);
global PFNglVertexArrayVertexBufferPROC __glVertexArrayVertexBuffer = NULL;
function inline void
glVertexArrayVertexBuffer (GLuint vaobj, GLuint bindingindex, GLuint buffer, GLintptr offset, GLsizei stride)
{
  __glVertexArrayVertexBuffer (vaobj, bindingindex, buffer, offset, stride);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglEnableVertexArrayAttribPROC)(GLuint vaobj, GLuint index);
global PFNglEnableVertexArrayAttribPROC __glEnableVertexArrayAttrib = NULL;
function inline void
glEnableVertexArrayAttrib (GLuint vaobj, GLuint index)
{
  __glEnableVertexArrayAttrib (vaobj, index);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglVertexArrayAttribFormatPROC)(GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLboolean normalized, GLuint relativeoffset);
global PFNglVertexArrayAttribFormatPROC __glVertexArrayAttribFormat = NULL;
function inline void
glVertexArrayAttribFormat (GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLboolean normalized, GLuint relativeoffset)
{
  __glVertexArrayAttribFormat (vaobj, attribindex, size, type, normalized, relativeoffset);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglVertexArrayAttribBindingPROC)(GLuint vaobj, GLuint attribindex, GLuint bindingindex);
global PFNglVertexArrayAttribBindingPROC __glVertexArrayAttribBinding = NULL;
function inline void
glVertexArrayAttribBinding (GLuint vaobj, GLuint attribindex, GLuint bindingindex)
{
  __glVertexArrayAttribBinding (vaobj, attribindex, bindingindex);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglBindVertexArrayPROC)(GLuint array);
global PFNglBindVertexArrayPROC __glBindVertexArray = NULL;
function inline void
glBindVertexArray (GLuint array)
{
  __glBindVertexArray (array);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglDeleteVertexArraysPROC)(GLsizei n, const GLuint *arrays);
global PFNglDeleteVertexArraysPROC __glDeleteVertexArrays = NULL;
function inline void
glDeleteVertexArrays (GLsizei n, const GLuint *arrays)
{
  __glDeleteVertexArrays (n, arrays);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglVertexArrayBindingDivisorPROC)(GLuint vaobj, GLuint bindingindex, GLuint divisor);
global PFNglVertexArrayBindingDivisorPROC __glVertexArrayBindingDivisor = NULL;
function inline void
glVertexArrayBindingDivisor (GLuint vaobj, GLuint bindingindex, GLuint divisor)
{
  __glVertexArrayBindingDivisor (vaobj, bindingindex, divisor);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglCreateTexturesPROC)(GLenum target, GLsizei n, GLuint *textures);
global PFNglCreateTexturesPROC __glCreateTextures = NULL;
function inline void
glCreateTextures (GLenum target, GLsizei n, GLuint *textures)
{
  __glCreateTextures (target, n, textures);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglTextureStorage2DPROC)(GLuint texture, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height);
global PFNglTextureStorage2DPROC __glTextureStorage2D = NULL;
function inline void
glTextureStorage2D (GLuint texture, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height)
{
  __glTextureStorage2D (texture, levels, internalformat, width, height);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglTextureSubImage2DPROC)(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *pixels);
global PFNglTextureSubImage2DPROC __glTextureSubImage2D = NULL;
function inline void
glTextureSubImage2D (GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *pixels)
{
  __glTextureSubImage2D (texture, level, xoffset, yoffset, width, height, format, type, pixels);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglTextureParameteriPROC)(GLuint texture, GLenum pname, GLint param);
global PFNglTextureParameteriPROC __glTextureParameteri = NULL;
function inline void
glTextureParameteri (GLuint texture, GLenum pname, GLint param)
{
  __glTextureParameteri (texture, pname, param);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglBindTextureUnitPROC)(GLuint unit, GLuint texture);
global PFNglBindTextureUnitPROC __glBindTextureUnit = NULL;
function inline void
glBindTextureUnit (GLuint unit, GLuint texture)
{
  __glBindTextureUnit (unit, texture);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglDeleteTexturesPROC)(GLsizei n, const GLuint *textures);
global PFNglDeleteTexturesPROC __glDeleteTextures = NULL;
function inline void
glDeleteTextures (GLsizei n, const GLuint *textures)
{
  __glDeleteTextures (n, textures);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglCreateFramebuffersPROC)(GLsizei n, GLuint *framebuffers);
global PFNglCreateFramebuffersPROC __glCreateFramebuffers = NULL;
function inline void
glCreateFramebuffers (GLsizei n, GLuint *framebuffers)
{
  __glCreateFramebuffers (n, framebuffers);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglNamedFramebufferTexturePROC)(GLuint framebuffer, GLenum attachment, GLuint texture, GLint level);
global PFNglNamedFramebufferTexturePROC __glNamedFramebufferTexture = NULL;
function inline void
glNamedFramebufferTexture (GLuint framebuffer, GLenum attachment, GLuint texture, GLint level)
{
  __glNamedFramebufferTexture (framebuffer, attachment, texture, level);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglNamedFramebufferRenderbufferPROC)(GLuint framebuffer, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
global PFNglNamedFramebufferRenderbufferPROC __glNamedFramebufferRenderbuffer = NULL;
function inline void
glNamedFramebufferRenderbuffer (GLuint framebuffer, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer)
{
  __glNamedFramebufferRenderbuffer (framebuffer, attachment, renderbuffertarget, renderbuffer);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglDeleteFramebuffersPROC)(GLsizei n, const GLuint *framebuffers);
global PFNglDeleteFramebuffersPROC __glDeleteFramebuffers = NULL;
function inline void
glDeleteFramebuffers (GLsizei n, const GLuint *framebuffers)
{
  __glDeleteFramebuffers (n, framebuffers);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglBindFramebufferPROC)(GLenum target, GLuint framebuffer);
global PFNglBindFramebufferPROC __glBindFramebuffer = NULL;
function inline void
glBindFramebuffer (GLenum target, GLuint framebuffer)
{
  __glBindFramebuffer (target, framebuffer);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglDrawArraysPROC)(GLenum mode, GLint first, GLsizei count);
global PFNglDrawArraysPROC __glDrawArrays = NULL;
function inline void
glDrawArrays (GLenum mode, GLint first, GLsizei count)
{
  __glDrawArrays (mode, first, count);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglDrawElementsPROC)(GLenum mode, GLsizei count, GLenum type, const void *indices);
global PFNglDrawElementsPROC __glDrawElements = NULL;
function inline void
glDrawElements (GLenum mode, GLsizei count, GLenum type, const void *indices)
{
  __glDrawElements (mode, count, type, indices);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglDrawArraysInstancedPROC)(GLenum mode, GLint first, GLsizei count, GLsizei instancecount);
global PFNglDrawArraysInstancedPROC __glDrawArraysInstanced = NULL;
function inline void
glDrawArraysInstanced (GLenum mode, GLint first, GLsizei count, GLsizei instancecount)
{
  __glDrawArraysInstanced (mode, first, count, instancecount);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglDrawElementsInstancedPROC)(GLenum mode, GLsizei count, GLenum type, const void *indices, GLsizei instancecount);
global PFNglDrawElementsInstancedPROC __glDrawElementsInstanced = NULL;
function inline void
glDrawElementsInstanced (GLenum mode, GLsizei count, GLenum type, const void *indices, GLsizei instancecount)
{
  __glDrawElementsInstanced (mode, count, type, indices, instancecount);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglClearColorPROC)(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
global PFNglClearColorPROC __glClearColor = NULL;
function inline void
glClearColor (GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
{
  __glClearColor (red, green, blue, alpha);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglDebugMessageCallbackPROC)(GLDEBUGPROC callback, const void *userParam);
global PFNglDebugMessageCallbackPROC __glDebugMessageCallback = NULL;
function inline void
glDebugMessageCallback (GLDEBUGPROC callback, const void *userParam)
{
  __glDebugMessageCallback (callback, userParam);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglEnablePROC)(GLenum cap);
global PFNglEnablePROC __glEnable = NULL;
function inline void
glEnable (GLenum cap)
{
  __glEnable (cap);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglGetIntegervPROC)(GLenum pname, GLint *data);
global PFNglGetIntegervPROC __glGetIntegerv = NULL;
function inline void
glGetIntegerv (GLenum pname, GLint *data)
{
  __glGetIntegerv (pname, data);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglVertexArrayAttribIFormatPROC)(GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset);
global PFNglVertexArrayAttribIFormatPROC __glVertexArrayAttribIFormat = NULL;
function inline void
glVertexArrayAttribIFormat (GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset)
{
  __glVertexArrayAttribIFormat (vaobj, attribindex, size, type, relativeoffset);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglUniform1ivPROC)(GLint location, GLsizei count, const GLint *value);
global PFNglUniform1ivPROC __glUniform1iv = NULL;
function inline void
glUniform1iv (GLint location, GLsizei count, const GLint *value)
{
  __glUniform1iv (location, count, value);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglUseProgramPROC)(GLuint program);
global PFNglUseProgramPROC __glUseProgram = NULL;
function inline void
glUseProgram (GLuint program)
{
  __glUseProgram (program);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglProgramUniform1ivPROC)(GLuint program, GLint location, GLsizei count, const GLint *value);
global PFNglProgramUniform1ivPROC __glProgramUniform1iv = NULL;
function inline void
glProgramUniform1iv (GLuint program, GLint location, GLsizei count, const GLint *value)
{
  __glProgramUniform1iv (program, location, count, value);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglPolygonModePROC)(GLenum face, GLenum mode);
global PFNglPolygonModePROC __glPolygonMode = NULL;
function inline void
glPolygonMode (GLenum face, GLenum mode)
{
  __glPolygonMode (face, mode);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglDepthRangefPROC)(GLfloat n, GLfloat f);
global PFNglDepthRangefPROC __glDepthRangef = NULL;
function inline void
glDepthRangef (GLfloat n, GLfloat f)
{
  __glDepthRangef (n, f);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglVertexArrayElementBufferPROC)(GLuint vaobj, GLuint buffer);
global PFNglVertexArrayElementBufferPROC __glVertexArrayElementBuffer = NULL;
function inline void
glVertexArrayElementBuffer (GLuint vaobj, GLuint buffer)
{
  __glVertexArrayElementBuffer (vaobj, buffer);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglProgramUniformMatrix3fvPROC)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
global PFNglProgramUniformMatrix3fvPROC __glProgramUniformMatrix3fv = NULL;
function inline void
glProgramUniformMatrix3fv (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)
{
  __glProgramUniformMatrix3fv (program, location, count, transpose, value);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglViewportPROC)(GLint x, GLint y, GLsizei width, GLsizei height);
global PFNglViewportPROC __glViewport = NULL;
function inline void
glViewport (GLint x, GLint y, GLsizei width, GLsizei height)
{
  __glViewport (x, y, width, height);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglBlendFuncPROC)(GLenum sfactor, GLenum dfactor);
global PFNglBlendFuncPROC __glBlendFunc = NULL;
function inline void
glBlendFunc (GLenum sfactor, GLenum dfactor)
{
  __glBlendFunc (sfactor, dfactor);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglClearPROC)(GLbitfield mask);
global PFNglClearPROC __glClear = NULL;
function inline void
glClear (GLbitfield mask)
{
  __glClear (mask);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglValidateProgramPipelinePROC)(GLuint pipeline);
global PFNglValidateProgramPipelinePROC __glValidateProgramPipeline = NULL;
function inline void
glValidateProgramPipeline (GLuint pipeline)
{
  __glValidateProgramPipeline (pipeline);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglGetProgramPipelineivPROC)(GLuint pipeline, GLenum pname, GLint *params);
global PFNglGetProgramPipelineivPROC __glGetProgramPipelineiv = NULL;
function inline void
glGetProgramPipelineiv (GLuint pipeline, GLenum pname, GLint *params)
{
  __glGetProgramPipelineiv (pipeline, pname, params);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglGetProgramPipelineInfoLogPROC)(GLuint pipeline, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
global PFNglGetProgramPipelineInfoLogPROC __glGetProgramPipelineInfoLog = NULL;
function inline void
glGetProgramPipelineInfoLog (GLuint pipeline, GLsizei bufSize, GLsizei *length, GLchar *infoLog)
{
  __glGetProgramPipelineInfoLog (pipeline, bufSize, length, infoLog);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglDepthMaskPROC)(GLboolean flag);
global PFNglDepthMaskPROC __glDepthMask = NULL;
function inline void
glDepthMask (GLboolean flag)
{
  __glDepthMask (flag);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglUniformMatrix4fvPROC)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
global PFNglUniformMatrix4fvPROC __glUniformMatrix4fv = NULL;
function inline void
glUniformMatrix4fv (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)
{
  __glUniformMatrix4fv (location, count, transpose, value);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglUniform1iPROC)(GLint location, GLint v0);
global PFNglUniform1iPROC __glUniform1i = NULL;
function inline void
glUniform1i (GLint location, GLint v0)
{
  __glUniform1i (location, v0);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglDepthFuncPROC)(GLenum func);
global PFNglDepthFuncPROC __glDepthFunc = NULL;
function inline void
glDepthFunc (GLenum func)
{
  __glDepthFunc (func);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglGenTexturesPROC)(GLsizei n, GLuint *textures);
global PFNglGenTexturesPROC __glGenTextures = NULL;
function inline void
glGenTextures (GLsizei n, GLuint *textures)
{
  __glGenTextures (n, textures);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglBindTexturePROC)(GLenum target, GLuint texture);
global PFNglBindTexturePROC __glBindTexture = NULL;
function inline void
glBindTexture (GLenum target, GLuint texture)
{
  __glBindTexture (target, texture);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglTexImage2DPROC)(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void *pixels);
global PFNglTexImage2DPROC __glTexImage2D = NULL;
function inline void
glTexImage2D (GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void *pixels)
{
  __glTexImage2D (target, level, internalformat, width, height, border, format, type, pixels);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglTexParameteriPROC)(GLenum target, GLenum pname, GLint param);
global PFNglTexParameteriPROC __glTexParameteri = NULL;
function inline void
glTexParameteri (GLenum target, GLenum pname, GLint param)
{
  __glTexParameteri (target, pname, param);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglPixelStoreiPROC)(GLenum pname, GLint param);
global PFNglPixelStoreiPROC __glPixelStorei = NULL;
function inline void
glPixelStorei (GLenum pname, GLint param)
{
  __glPixelStorei (pname, param);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglGenerateMipmapPROC)(GLenum target);
global PFNglGenerateMipmapPROC __glGenerateMipmap = NULL;
function inline void
glGenerateMipmap (GLenum target)
{
  __glGenerateMipmap (target);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglDisablePROC)(GLenum cap);
global PFNglDisablePROC __glDisable = NULL;
function inline void
glDisable (GLenum cap)
{
  __glDisable (cap);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglGenFramebuffersPROC)(GLsizei n, GLuint *framebuffers);
global PFNglGenFramebuffersPROC __glGenFramebuffers = NULL;
function inline void
glGenFramebuffers (GLsizei n, GLuint *framebuffers)
{
  __glGenFramebuffers (n, framebuffers);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglFramebufferTexture2DPROC)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
global PFNglFramebufferTexture2DPROC __glFramebufferTexture2D = NULL;
function inline void
glFramebufferTexture2D (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level)
{
  __glFramebufferTexture2D (target, attachment, textarget, texture, level);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglGetTexLevelParameterivPROC)(GLenum target, GLint level, GLenum pname, GLint *params);
global PFNglGetTexLevelParameterivPROC __glGetTexLevelParameteriv = NULL;
function inline void
glGetTexLevelParameteriv (GLenum target, GLint level, GLenum pname, GLint *params)
{
  __glGetTexLevelParameteriv (target, level, pname, params);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglReadBufferPROC)(GLenum src);
global PFNglReadBufferPROC __glReadBuffer = NULL;
function inline void
glReadBuffer (GLenum src)
{
  __glReadBuffer (src);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglReadPixelsPROC)(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, void *pixels);
global PFNglReadPixelsPROC __glReadPixels = NULL;
function inline void
glReadPixels (GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, void *pixels)
{
  __glReadPixels (x, y, width, height, format, type, pixels);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglGenQueriesPROC)(GLsizei n, GLuint *ids);
global PFNglGenQueriesPROC __glGenQueries = NULL;
function inline void
glGenQueries (GLsizei n, GLuint *ids)
{
  __glGenQueries (n, ids);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglGetQueryObjectui64vPROC)(GLuint id, GLenum pname, GLuint64 *params);
global PFNglGetQueryObjectui64vPROC __glGetQueryObjectui64v = NULL;
function inline void
glGetQueryObjectui64v (GLuint id, GLenum pname, GLuint64 *params)
{
  __glGetQueryObjectui64v (id, pname, params);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglBeginQueryPROC)(GLenum target, GLuint id);
global PFNglBeginQueryPROC __glBeginQuery = NULL;
function inline void
glBeginQuery (GLenum target, GLuint id)
{
  __glBeginQuery (target, id);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglEndQueryPROC)(GLenum target);
global PFNglEndQueryPROC __glEndQuery = NULL;
function inline void
glEndQuery (GLenum target)
{
  __glEndQuery (target);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglBlendFuncSeparatePROC)(GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha);
global PFNglBlendFuncSeparatePROC __glBlendFuncSeparate = NULL;
function inline void
glBlendFuncSeparate (GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha)
{
  __glBlendFuncSeparate (sfactorRGB, dfactorRGB, sfactorAlpha, dfactorAlpha);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglFinishPROC)();
global PFNglFinishPROC __glFinish = NULL;
function inline void
glFinish ()
{
  __glFinish ();
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglObjectLabelPROC)(GLenum identifier, GLuint name, GLsizei length, const char* label);
global PFNglObjectLabelPROC __glObjectLabel = NULL;
function inline void
glObjectLabel (GLenum identifier, GLuint name, GLsizei length, const char* label)
{
  __glObjectLabel (identifier, name, length, label);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglPushDebugGroupPROC)(GLenum source, GLuint id, GLsizei length, const GLchar *message);
global PFNglPushDebugGroupPROC __glPushDebugGroup = NULL;
function inline void
glPushDebugGroup (GLenum source, GLuint id, GLsizei length, const GLchar *message)
{
  __glPushDebugGroup (source, id, length, message);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglPopDebugGroupPROC)();
global PFNglPopDebugGroupPROC __glPopDebugGroup = NULL;
function inline void
glPopDebugGroup ()
{
  __glPopDebugGroup ();
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglPointSizePROC)(GLfloat size);
global PFNglPointSizePROC __glPointSize = NULL;
function inline void
glPointSize (GLfloat size)
{
  __glPointSize (size);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglLineWidthPROC)(GLfloat width);
global PFNglLineWidthPROC __glLineWidth = NULL;
function inline void
glLineWidth (GLfloat width)
{
  __glLineWidth (width);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglClearTexImagePROC)(GLuint texture, GLint level, GLenum format, GLenum type, const void *data);
global PFNglClearTexImagePROC __glClearTexImage = NULL;
function inline void
glClearTexImage (GLuint texture, GLint level, GLenum format, GLenum type, const void *data)
{
  __glClearTexImage (texture, level, format, type, data);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglBindBufferBasePROC)(GLenum target, GLuint index, GLuint buffer);
global PFNglBindBufferBasePROC __glBindBufferBase = NULL;
function inline void
glBindBufferBase (GLenum target, GLuint index, GLuint buffer)
{
  __glBindBufferBase (target, index, buffer);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglTextureStorage3DPROC)(GLuint texture, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth);
global PFNglTextureStorage3DPROC __glTextureStorage3D = NULL;
function inline void
glTextureStorage3D (GLuint texture, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth)
{
  __glTextureStorage3D (texture, levels, internalformat, width, height, depth);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglTextureSubImage3DPROC)(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void *pixels);
global PFNglTextureSubImage3DPROC __glTextureSubImage3D = NULL;
function inline void
glTextureSubImage3D (GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void *pixels)
{
  __glTextureSubImage3D (texture, level, xoffset, yoffset, zoffset, width, height, depth, format, type, pixels);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
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

typedef GLenum (*PFNglCheckFramebufferStatusPROC)(GLenum target);
global PFNglCheckFramebufferStatusPROC __glCheckFramebufferStatus = NULL;
function inline GLenum
glCheckFramebufferStatus (GLenum target)
{
  return __glCheckFramebufferStatus (target);
}

#endif // OPENGL_FUNCTIONS_H