/* Generated code */

#ifndef OPENGL_FUNCTIONS_H
#define OPENGL_FUNCTIONS_H

fz_function b32 _opengl_load_functions(); /* Loads all opengl functions in the on the code gen Table */

typedef void (*PFNglProgramUniform1iPROC)(GLuint program, GLint location, GLint v0);
fz_global PFNglProgramUniform1iPROC __glProgramUniform1i = NULL;
fz_function inline void
glProgramUniform1i (GLuint program, GLint location, GLint v0)
{
  __glProgramUniform1i (program, location, v0);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglProgramUniform1fPROC)(GLuint program, GLint location, GLfloat v0);
fz_global PFNglProgramUniform1fPROC __glProgramUniform1f = NULL;
fz_function inline void
glProgramUniform1f (GLuint program, GLint location, GLfloat v0)
{
  __glProgramUniform1f (program, location, v0);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglProgramUniform2fPROC)(GLuint program, GLint location, GLfloat v0, GLfloat v1);
fz_global PFNglProgramUniform2fPROC __glProgramUniform2f = NULL;
fz_function inline void
glProgramUniform2f (GLuint program, GLint location, GLfloat v0, GLfloat v1)
{
  __glProgramUniform2f (program, location, v0, v1);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglProgramUniform3fPROC)(GLuint program, GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
fz_global PFNglProgramUniform3fPROC __glProgramUniform3f = NULL;
fz_function inline void
glProgramUniform3f (GLuint program, GLint location, GLfloat v0, GLfloat v1, GLfloat v2)
{
  __glProgramUniform3f (program, location, v0, v1, v2);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglProgramUniform4fPROC)(GLuint program, GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
fz_global PFNglProgramUniform4fPROC __glProgramUniform4f = NULL;
fz_function inline void
glProgramUniform4f (GLuint program, GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3)
{
  __glProgramUniform4f (program, location, v0, v1, v2, v3);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglProgramUniform3fvPROC)(GLuint program, GLint location, GLsizei count, const GLfloat *value);
fz_global PFNglProgramUniform3fvPROC __glProgramUniform3fv = NULL;
fz_function inline void
glProgramUniform3fv (GLuint program, GLint location, GLsizei count, const GLfloat *value)
{
  __glProgramUniform3fv (program, location, count, value);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglProgramUniformMatrix4fvPROC)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
fz_global PFNglProgramUniformMatrix4fvPROC __glProgramUniformMatrix4fv = NULL;
fz_function inline void
glProgramUniformMatrix4fv (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)
{
  __glProgramUniformMatrix4fv (program, location, count, transpose, value);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglProgramUniformMatrix2fvPROC)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
fz_global PFNglProgramUniformMatrix2fvPROC __glProgramUniformMatrix2fv = NULL;
fz_function inline void
glProgramUniformMatrix2fv (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)
{
  __glProgramUniformMatrix2fv (program, location, count, transpose, value);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglAttachShaderPROC)(GLuint program, GLuint shader);
fz_global PFNglAttachShaderPROC __glAttachShader = NULL;
fz_function inline void
glAttachShader (GLuint program, GLuint shader)
{
  __glAttachShader (program, shader);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglLinkProgramPROC)(GLuint program);
fz_global PFNglLinkProgramPROC __glLinkProgram = NULL;
fz_function inline void
glLinkProgram (GLuint program)
{
  __glLinkProgram (program);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglDeleteShaderPROC)(GLuint shader);
fz_global PFNglDeleteShaderPROC __glDeleteShader = NULL;
fz_function inline void
glDeleteShader (GLuint shader)
{
  __glDeleteShader (shader);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglDeleteProgramPROC)(GLuint program);
fz_global PFNglDeleteProgramPROC __glDeleteProgram = NULL;
fz_function inline void
glDeleteProgram (GLuint program)
{
  __glDeleteProgram (program);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglGetProgramivPROC)(GLuint program, GLenum pname, GLint *params);
fz_global PFNglGetProgramivPROC __glGetProgramiv = NULL;
fz_function inline void
glGetProgramiv (GLuint program, GLenum pname, GLint *params)
{
  __glGetProgramiv (program, pname, params);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglGetProgramInfoLogPROC)(GLuint program, GLsizei bufSize, GLsizei *length, char *infoLog);
fz_global PFNglGetProgramInfoLogPROC __glGetProgramInfoLog = NULL;
fz_function inline void
glGetProgramInfoLog (GLuint program, GLsizei bufSize, GLsizei *length, char *infoLog)
{
  __glGetProgramInfoLog (program, bufSize, length, infoLog);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglGenProgramPipelinesPROC)(GLsizei n, GLuint *pipelines);
fz_global PFNglGenProgramPipelinesPROC __glGenProgramPipelines = NULL;
fz_function inline void
glGenProgramPipelines (GLsizei n, GLuint *pipelines)
{
  __glGenProgramPipelines (n, pipelines);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglBindProgramPipelinePROC)(GLuint pipeline);
fz_global PFNglBindProgramPipelinePROC __glBindProgramPipeline = NULL;
fz_function inline void
glBindProgramPipeline (GLuint pipeline)
{
  __glBindProgramPipeline (pipeline);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglUseProgramStagesPROC)(GLuint pipeline, GLbitfield stages, GLuint program);
fz_global PFNglUseProgramStagesPROC __glUseProgramStages = NULL;
fz_function inline void
glUseProgramStages (GLuint pipeline, GLbitfield stages, GLuint program)
{
  __glUseProgramStages (pipeline, stages, program);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglDeleteProgramPipelinesPROC)(GLsizei n, const GLuint *pipelines);
fz_global PFNglDeleteProgramPipelinesPROC __glDeleteProgramPipelines = NULL;
fz_function inline void
glDeleteProgramPipelines (GLsizei n, const GLuint *pipelines)
{
  __glDeleteProgramPipelines (n, pipelines);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglCreateBuffersPROC)(GLsizei n, GLuint *buffers);
fz_global PFNglCreateBuffersPROC __glCreateBuffers = NULL;
fz_function inline void
glCreateBuffers (GLsizei n, GLuint *buffers)
{
  __glCreateBuffers (n, buffers);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglNamedBufferDataPROC)(GLuint buffer, GLsizeiptr size, const void *data, GLenum usage);
fz_global PFNglNamedBufferDataPROC __glNamedBufferData = NULL;
fz_function inline void
glNamedBufferData (GLuint buffer, GLsizeiptr size, const void *data, GLenum usage)
{
  __glNamedBufferData (buffer, size, data, usage);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglNamedBufferSubDataPROC)(GLuint buffer, GLintptr offset, GLsizeiptr size, const void *data);
fz_global PFNglNamedBufferSubDataPROC __glNamedBufferSubData = NULL;
fz_function inline void
glNamedBufferSubData (GLuint buffer, GLintptr offset, GLsizeiptr size, const void *data)
{
  __glNamedBufferSubData (buffer, offset, size, data);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglDeleteBuffersPROC)(GLsizei n, const GLuint *buffers);
fz_global PFNglDeleteBuffersPROC __glDeleteBuffers = NULL;
fz_function inline void
glDeleteBuffers (GLsizei n, const GLuint *buffers)
{
  __glDeleteBuffers (n, buffers);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglBindBufferPROC)(GLenum target, GLuint buffer);
fz_global PFNglBindBufferPROC __glBindBuffer = NULL;
fz_function inline void
glBindBuffer (GLenum target, GLuint buffer)
{
  __glBindBuffer (target, buffer);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglNamedBufferStoragePROC)(GLuint buffer, GLsizeiptr size, const void *data, GLbitfield flags);
fz_global PFNglNamedBufferStoragePROC __glNamedBufferStorage = NULL;
fz_function inline void
glNamedBufferStorage (GLuint buffer, GLsizeiptr size, const void *data, GLbitfield flags)
{
  __glNamedBufferStorage (buffer, size, data, flags);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglCreateProgramPipelinesPROC)(GLsizei n, GLuint *pipelines);
fz_global PFNglCreateProgramPipelinesPROC __glCreateProgramPipelines = NULL;
fz_function inline void
glCreateProgramPipelines (GLsizei n, GLuint *pipelines)
{
  __glCreateProgramPipelines (n, pipelines);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglCreateVertexArraysPROC)(GLsizei n, GLuint *arrays);
fz_global PFNglCreateVertexArraysPROC __glCreateVertexArrays = NULL;
fz_function inline void
glCreateVertexArrays (GLsizei n, GLuint *arrays)
{
  __glCreateVertexArrays (n, arrays);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglVertexArrayVertexBufferPROC)(GLuint vaobj, GLuint bindingindex, GLuint buffer, GLintptr offset, GLsizei stride);
fz_global PFNglVertexArrayVertexBufferPROC __glVertexArrayVertexBuffer = NULL;
fz_function inline void
glVertexArrayVertexBuffer (GLuint vaobj, GLuint bindingindex, GLuint buffer, GLintptr offset, GLsizei stride)
{
  __glVertexArrayVertexBuffer (vaobj, bindingindex, buffer, offset, stride);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglEnableVertexArrayAttribPROC)(GLuint vaobj, GLuint index);
fz_global PFNglEnableVertexArrayAttribPROC __glEnableVertexArrayAttrib = NULL;
fz_function inline void
glEnableVertexArrayAttrib (GLuint vaobj, GLuint index)
{
  __glEnableVertexArrayAttrib (vaobj, index);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglVertexArrayAttribFormatPROC)(GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLboolean normalized, GLuint relativeoffset);
fz_global PFNglVertexArrayAttribFormatPROC __glVertexArrayAttribFormat = NULL;
fz_function inline void
glVertexArrayAttribFormat (GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLboolean normalized, GLuint relativeoffset)
{
  __glVertexArrayAttribFormat (vaobj, attribindex, size, type, normalized, relativeoffset);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglVertexArrayAttribBindingPROC)(GLuint vaobj, GLuint attribindex, GLuint bindingindex);
fz_global PFNglVertexArrayAttribBindingPROC __glVertexArrayAttribBinding = NULL;
fz_function inline void
glVertexArrayAttribBinding (GLuint vaobj, GLuint attribindex, GLuint bindingindex)
{
  __glVertexArrayAttribBinding (vaobj, attribindex, bindingindex);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglBindVertexArrayPROC)(GLuint array);
fz_global PFNglBindVertexArrayPROC __glBindVertexArray = NULL;
fz_function inline void
glBindVertexArray (GLuint array)
{
  __glBindVertexArray (array);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglDeleteVertexArraysPROC)(GLsizei n, const GLuint *arrays);
fz_global PFNglDeleteVertexArraysPROC __glDeleteVertexArrays = NULL;
fz_function inline void
glDeleteVertexArrays (GLsizei n, const GLuint *arrays)
{
  __glDeleteVertexArrays (n, arrays);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglVertexArrayBindingDivisorPROC)(GLuint vaobj, GLuint bindingindex, GLuint divisor);
fz_global PFNglVertexArrayBindingDivisorPROC __glVertexArrayBindingDivisor = NULL;
fz_function inline void
glVertexArrayBindingDivisor (GLuint vaobj, GLuint bindingindex, GLuint divisor)
{
  __glVertexArrayBindingDivisor (vaobj, bindingindex, divisor);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglCreateTexturesPROC)(GLenum target, GLsizei n, GLuint *textures);
fz_global PFNglCreateTexturesPROC __glCreateTextures = NULL;
fz_function inline void
glCreateTextures (GLenum target, GLsizei n, GLuint *textures)
{
  __glCreateTextures (target, n, textures);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglTextureStorage2DPROC)(GLuint texture, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height);
fz_global PFNglTextureStorage2DPROC __glTextureStorage2D = NULL;
fz_function inline void
glTextureStorage2D (GLuint texture, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height)
{
  __glTextureStorage2D (texture, levels, internalformat, width, height);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglTextureSubImage2DPROC)(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *pixels);
fz_global PFNglTextureSubImage2DPROC __glTextureSubImage2D = NULL;
fz_function inline void
glTextureSubImage2D (GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *pixels)
{
  __glTextureSubImage2D (texture, level, xoffset, yoffset, width, height, format, type, pixels);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglTextureParameteriPROC)(GLuint texture, GLenum pname, GLint param);
fz_global PFNglTextureParameteriPROC __glTextureParameteri = NULL;
fz_function inline void
glTextureParameteri (GLuint texture, GLenum pname, GLint param)
{
  __glTextureParameteri (texture, pname, param);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglBindTextureUnitPROC)(GLuint unit, GLuint texture);
fz_global PFNglBindTextureUnitPROC __glBindTextureUnit = NULL;
fz_function inline void
glBindTextureUnit (GLuint unit, GLuint texture)
{
  __glBindTextureUnit (unit, texture);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglDeleteTexturesPROC)(GLsizei n, const GLuint *textures);
fz_global PFNglDeleteTexturesPROC __glDeleteTextures = NULL;
fz_function inline void
glDeleteTextures (GLsizei n, const GLuint *textures)
{
  __glDeleteTextures (n, textures);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglCreateFramebuffersPROC)(GLsizei n, GLuint *framebuffers);
fz_global PFNglCreateFramebuffersPROC __glCreateFramebuffers = NULL;
fz_function inline void
glCreateFramebuffers (GLsizei n, GLuint *framebuffers)
{
  __glCreateFramebuffers (n, framebuffers);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglNamedFramebufferTexturePROC)(GLuint framebuffer, GLenum attachment, GLuint texture, GLint level);
fz_global PFNglNamedFramebufferTexturePROC __glNamedFramebufferTexture = NULL;
fz_function inline void
glNamedFramebufferTexture (GLuint framebuffer, GLenum attachment, GLuint texture, GLint level)
{
  __glNamedFramebufferTexture (framebuffer, attachment, texture, level);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglNamedFramebufferRenderbufferPROC)(GLuint framebuffer, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
fz_global PFNglNamedFramebufferRenderbufferPROC __glNamedFramebufferRenderbuffer = NULL;
fz_function inline void
glNamedFramebufferRenderbuffer (GLuint framebuffer, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer)
{
  __glNamedFramebufferRenderbuffer (framebuffer, attachment, renderbuffertarget, renderbuffer);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglDeleteFramebuffersPROC)(GLsizei n, const GLuint *framebuffers);
fz_global PFNglDeleteFramebuffersPROC __glDeleteFramebuffers = NULL;
fz_function inline void
glDeleteFramebuffers (GLsizei n, const GLuint *framebuffers)
{
  __glDeleteFramebuffers (n, framebuffers);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglBindFramebufferPROC)(GLenum target, GLuint framebuffer);
fz_global PFNglBindFramebufferPROC __glBindFramebuffer = NULL;
fz_function inline void
glBindFramebuffer (GLenum target, GLuint framebuffer)
{
  __glBindFramebuffer (target, framebuffer);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglDrawArraysPROC)(GLenum mode, GLint first, GLsizei count);
fz_global PFNglDrawArraysPROC __glDrawArrays = NULL;
fz_function inline void
glDrawArrays (GLenum mode, GLint first, GLsizei count)
{
  __glDrawArrays (mode, first, count);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglDrawElementsPROC)(GLenum mode, GLsizei count, GLenum type, const void *indices);
fz_global PFNglDrawElementsPROC __glDrawElements = NULL;
fz_function inline void
glDrawElements (GLenum mode, GLsizei count, GLenum type, const void *indices)
{
  __glDrawElements (mode, count, type, indices);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglDrawArraysInstancedPROC)(GLenum mode, GLint first, GLsizei count, GLsizei instancecount);
fz_global PFNglDrawArraysInstancedPROC __glDrawArraysInstanced = NULL;
fz_function inline void
glDrawArraysInstanced (GLenum mode, GLint first, GLsizei count, GLsizei instancecount)
{
  __glDrawArraysInstanced (mode, first, count, instancecount);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglDrawElementsInstancedPROC)(GLenum mode, GLsizei count, GLenum type, const void *indices, GLsizei instancecount);
fz_global PFNglDrawElementsInstancedPROC __glDrawElementsInstanced = NULL;
fz_function inline void
glDrawElementsInstanced (GLenum mode, GLsizei count, GLenum type, const void *indices, GLsizei instancecount)
{
  __glDrawElementsInstanced (mode, count, type, indices, instancecount);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglClearColorPROC)(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
fz_global PFNglClearColorPROC __glClearColor = NULL;
fz_function inline void
glClearColor (GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
{
  __glClearColor (red, green, blue, alpha);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglDebugMessageCallbackPROC)(GLDEBUGPROC callback, const void *userParam);
fz_global PFNglDebugMessageCallbackPROC __glDebugMessageCallback = NULL;
fz_function inline void
glDebugMessageCallback (GLDEBUGPROC callback, const void *userParam)
{
  __glDebugMessageCallback (callback, userParam);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglEnablePROC)(GLenum cap);
fz_global PFNglEnablePROC __glEnable = NULL;
fz_function inline void
glEnable (GLenum cap)
{
  __glEnable (cap);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglGetIntegervPROC)(GLenum pname, GLint *data);
fz_global PFNglGetIntegervPROC __glGetIntegerv = NULL;
fz_function inline void
glGetIntegerv (GLenum pname, GLint *data)
{
  __glGetIntegerv (pname, data);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglVertexArrayAttribIFormatPROC)(GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset);
fz_global PFNglVertexArrayAttribIFormatPROC __glVertexArrayAttribIFormat = NULL;
fz_function inline void
glVertexArrayAttribIFormat (GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset)
{
  __glVertexArrayAttribIFormat (vaobj, attribindex, size, type, relativeoffset);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglUniform1ivPROC)(GLint location, GLsizei count, const GLint *value);
fz_global PFNglUniform1ivPROC __glUniform1iv = NULL;
fz_function inline void
glUniform1iv (GLint location, GLsizei count, const GLint *value)
{
  __glUniform1iv (location, count, value);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglUseProgramPROC)(GLuint program);
fz_global PFNglUseProgramPROC __glUseProgram = NULL;
fz_function inline void
glUseProgram (GLuint program)
{
  __glUseProgram (program);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglProgramUniform1ivPROC)(GLuint program, GLint location, GLsizei count, const GLint *value);
fz_global PFNglProgramUniform1ivPROC __glProgramUniform1iv = NULL;
fz_function inline void
glProgramUniform1iv (GLuint program, GLint location, GLsizei count, const GLint *value)
{
  __glProgramUniform1iv (program, location, count, value);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglPolygonModePROC)(GLenum face, GLenum mode);
fz_global PFNglPolygonModePROC __glPolygonMode = NULL;
fz_function inline void
glPolygonMode (GLenum face, GLenum mode)
{
  __glPolygonMode (face, mode);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglDepthRangefPROC)(GLfloat n, GLfloat f);
fz_global PFNglDepthRangefPROC __glDepthRangef = NULL;
fz_function inline void
glDepthRangef (GLfloat n, GLfloat f)
{
  __glDepthRangef (n, f);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglVertexArrayElementBufferPROC)(GLuint vaobj, GLuint buffer);
fz_global PFNglVertexArrayElementBufferPROC __glVertexArrayElementBuffer = NULL;
fz_function inline void
glVertexArrayElementBuffer (GLuint vaobj, GLuint buffer)
{
  __glVertexArrayElementBuffer (vaobj, buffer);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglProgramUniformMatrix3fvPROC)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
fz_global PFNglProgramUniformMatrix3fvPROC __glProgramUniformMatrix3fv = NULL;
fz_function inline void
glProgramUniformMatrix3fv (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)
{
  __glProgramUniformMatrix3fv (program, location, count, transpose, value);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglViewportPROC)(GLint x, GLint y, GLsizei width, GLsizei height);
fz_global PFNglViewportPROC __glViewport = NULL;
fz_function inline void
glViewport (GLint x, GLint y, GLsizei width, GLsizei height)
{
  __glViewport (x, y, width, height);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglBlendFuncPROC)(GLenum sfactor, GLenum dfactor);
fz_global PFNglBlendFuncPROC __glBlendFunc = NULL;
fz_function inline void
glBlendFunc (GLenum sfactor, GLenum dfactor)
{
  __glBlendFunc (sfactor, dfactor);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglClearPROC)(GLbitfield mask);
fz_global PFNglClearPROC __glClear = NULL;
fz_function inline void
glClear (GLbitfield mask)
{
  __glClear (mask);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglValidateProgramPipelinePROC)(GLuint pipeline);
fz_global PFNglValidateProgramPipelinePROC __glValidateProgramPipeline = NULL;
fz_function inline void
glValidateProgramPipeline (GLuint pipeline)
{
  __glValidateProgramPipeline (pipeline);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglGetProgramPipelineivPROC)(GLuint pipeline, GLenum pname, GLint *params);
fz_global PFNglGetProgramPipelineivPROC __glGetProgramPipelineiv = NULL;
fz_function inline void
glGetProgramPipelineiv (GLuint pipeline, GLenum pname, GLint *params)
{
  __glGetProgramPipelineiv (pipeline, pname, params);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglGetProgramPipelineInfoLogPROC)(GLuint pipeline, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
fz_global PFNglGetProgramPipelineInfoLogPROC __glGetProgramPipelineInfoLog = NULL;
fz_function inline void
glGetProgramPipelineInfoLog (GLuint pipeline, GLsizei bufSize, GLsizei *length, GLchar *infoLog)
{
  __glGetProgramPipelineInfoLog (pipeline, bufSize, length, infoLog);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglDepthMaskPROC)(GLboolean flag);
fz_global PFNglDepthMaskPROC __glDepthMask = NULL;
fz_function inline void
glDepthMask (GLboolean flag)
{
  __glDepthMask (flag);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglUniformMatrix4fvPROC)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
fz_global PFNglUniformMatrix4fvPROC __glUniformMatrix4fv = NULL;
fz_function inline void
glUniformMatrix4fv (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)
{
  __glUniformMatrix4fv (location, count, transpose, value);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglUniform1iPROC)(GLint location, GLint v0);
fz_global PFNglUniform1iPROC __glUniform1i = NULL;
fz_function inline void
glUniform1i (GLint location, GLint v0)
{
  __glUniform1i (location, v0);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglDepthFuncPROC)(GLenum func);
fz_global PFNglDepthFuncPROC __glDepthFunc = NULL;
fz_function inline void
glDepthFunc (GLenum func)
{
  __glDepthFunc (func);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglGenTexturesPROC)(GLsizei n, GLuint *textures);
fz_global PFNglGenTexturesPROC __glGenTextures = NULL;
fz_function inline void
glGenTextures (GLsizei n, GLuint *textures)
{
  __glGenTextures (n, textures);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglBindTexturePROC)(GLenum target, GLuint texture);
fz_global PFNglBindTexturePROC __glBindTexture = NULL;
fz_function inline void
glBindTexture (GLenum target, GLuint texture)
{
  __glBindTexture (target, texture);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglTexImage2DPROC)(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void *pixels);
fz_global PFNglTexImage2DPROC __glTexImage2D = NULL;
fz_function inline void
glTexImage2D (GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void *pixels)
{
  __glTexImage2D (target, level, internalformat, width, height, border, format, type, pixels);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglTexParameteriPROC)(GLenum target, GLenum pname, GLint param);
fz_global PFNglTexParameteriPROC __glTexParameteri = NULL;
fz_function inline void
glTexParameteri (GLenum target, GLenum pname, GLint param)
{
  __glTexParameteri (target, pname, param);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglPixelStoreiPROC)(GLenum pname, GLint param);
fz_global PFNglPixelStoreiPROC __glPixelStorei = NULL;
fz_function inline void
glPixelStorei (GLenum pname, GLint param)
{
  __glPixelStorei (pname, param);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglGenerateMipmapPROC)(GLenum target);
fz_global PFNglGenerateMipmapPROC __glGenerateMipmap = NULL;
fz_function inline void
glGenerateMipmap (GLenum target)
{
  __glGenerateMipmap (target);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglDisablePROC)(GLenum cap);
fz_global PFNglDisablePROC __glDisable = NULL;
fz_function inline void
glDisable (GLenum cap)
{
  __glDisable (cap);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglGenFramebuffersPROC)(GLsizei n, GLuint *framebuffers);
fz_global PFNglGenFramebuffersPROC __glGenFramebuffers = NULL;
fz_function inline void
glGenFramebuffers (GLsizei n, GLuint *framebuffers)
{
  __glGenFramebuffers (n, framebuffers);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglFramebufferTexture2DPROC)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
fz_global PFNglFramebufferTexture2DPROC __glFramebufferTexture2D = NULL;
fz_function inline void
glFramebufferTexture2D (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level)
{
  __glFramebufferTexture2D (target, attachment, textarget, texture, level);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglGetTexLevelParameterivPROC)(GLenum target, GLint level, GLenum pname, GLint *params);
fz_global PFNglGetTexLevelParameterivPROC __glGetTexLevelParameteriv = NULL;
fz_function inline void
glGetTexLevelParameteriv (GLenum target, GLint level, GLenum pname, GLint *params)
{
  __glGetTexLevelParameteriv (target, level, pname, params);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglReadBufferPROC)(GLenum src);
fz_global PFNglReadBufferPROC __glReadBuffer = NULL;
fz_function inline void
glReadBuffer (GLenum src)
{
  __glReadBuffer (src);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglReadPixelsPROC)(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, void *pixels);
fz_global PFNglReadPixelsPROC __glReadPixels = NULL;
fz_function inline void
glReadPixels (GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, void *pixels)
{
  __glReadPixels (x, y, width, height, format, type, pixels);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglGenQueriesPROC)(GLsizei n, GLuint *ids);
fz_global PFNglGenQueriesPROC __glGenQueries = NULL;
fz_function inline void
glGenQueries (GLsizei n, GLuint *ids)
{
  __glGenQueries (n, ids);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglGetQueryObjectui64vPROC)(GLuint id, GLenum pname, GLuint64 *params);
fz_global PFNglGetQueryObjectui64vPROC __glGetQueryObjectui64v = NULL;
fz_function inline void
glGetQueryObjectui64v (GLuint id, GLenum pname, GLuint64 *params)
{
  __glGetQueryObjectui64v (id, pname, params);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglBeginQueryPROC)(GLenum target, GLuint id);
fz_global PFNglBeginQueryPROC __glBeginQuery = NULL;
fz_function inline void
glBeginQuery (GLenum target, GLuint id)
{
  __glBeginQuery (target, id);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglEndQueryPROC)(GLenum target);
fz_global PFNglEndQueryPROC __glEndQuery = NULL;
fz_function inline void
glEndQuery (GLenum target)
{
  __glEndQuery (target);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglBlendFuncSeparatePROC)(GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha);
fz_global PFNglBlendFuncSeparatePROC __glBlendFuncSeparate = NULL;
fz_function inline void
glBlendFuncSeparate (GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha)
{
  __glBlendFuncSeparate (sfactorRGB, dfactorRGB, sfactorAlpha, dfactorAlpha);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglFinishPROC)();
fz_global PFNglFinishPROC __glFinish = NULL;
fz_function inline void
glFinish ()
{
  __glFinish ();
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglObjectLabelPROC)(GLenum identifier, GLuint name, GLsizei length, const char* label);
fz_global PFNglObjectLabelPROC __glObjectLabel = NULL;
fz_function inline void
glObjectLabel (GLenum identifier, GLuint name, GLsizei length, const char* label)
{
  __glObjectLabel (identifier, name, length, label);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglPushDebugGroupPROC)(GLenum source, GLuint id, GLsizei length, const GLchar *message);
fz_global PFNglPushDebugGroupPROC __glPushDebugGroup = NULL;
fz_function inline void
glPushDebugGroup (GLenum source, GLuint id, GLsizei length, const GLchar *message)
{
  __glPushDebugGroup (source, id, length, message);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglPopDebugGroupPROC)();
fz_global PFNglPopDebugGroupPROC __glPopDebugGroup = NULL;
fz_function inline void
glPopDebugGroup ()
{
  __glPopDebugGroup ();
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglPointSizePROC)(GLfloat size);
fz_global PFNglPointSizePROC __glPointSize = NULL;
fz_function inline void
glPointSize (GLfloat size)
{
  __glPointSize (size);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglLineWidthPROC)(GLfloat width);
fz_global PFNglLineWidthPROC __glLineWidth = NULL;
fz_function inline void
glLineWidth (GLfloat width)
{
  __glLineWidth (width);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglClearTexImagePROC)(GLuint texture, GLint level, GLenum format, GLenum type, const void *data);
fz_global PFNglClearTexImagePROC __glClearTexImage = NULL;
fz_function inline void
glClearTexImage (GLuint texture, GLint level, GLenum format, GLenum type, const void *data)
{
  __glClearTexImage (texture, level, format, type, data);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglBindBufferBasePROC)(GLenum target, GLuint index, GLuint buffer);
fz_global PFNglBindBufferBasePROC __glBindBufferBase = NULL;
fz_function inline void
glBindBufferBase (GLenum target, GLuint index, GLuint buffer)
{
  __glBindBufferBase (target, index, buffer);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglTextureStorage3DPROC)(GLuint texture, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth);
fz_global PFNglTextureStorage3DPROC __glTextureStorage3D = NULL;
fz_function inline void
glTextureStorage3D (GLuint texture, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth)
{
  __glTextureStorage3D (texture, levels, internalformat, width, height, depth);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef void (*PFNglTextureSubImage3DPROC)(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void *pixels);
fz_global PFNglTextureSubImage3DPROC __glTextureSubImage3D = NULL;
fz_function inline void
glTextureSubImage3D (GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void *pixels)
{
  __glTextureSubImage3D (texture, level, xoffset, yoffset, zoffset, width, height, depth, format, type, pixels);
#if DEBUG_OPENGL_CHECK_ERRORS
  opengl_check_errors();
#endif
}

typedef GLuint (*PFNglCreateShaderProgramvPROC)(GLenum type, GLsizei count, const char *const* strings);
fz_global PFNglCreateShaderProgramvPROC __glCreateShaderProgramv = NULL;
fz_function inline GLuint
glCreateShaderProgramv (GLenum type, GLsizei count, const char *const* strings)
{
  return __glCreateShaderProgramv (type, count, strings);
}

typedef GLint (*PFNglGetUniformLocationPROC)(GLuint program, const GLchar *name);
fz_global PFNglGetUniformLocationPROC __glGetUniformLocation = NULL;
fz_function inline GLint
glGetUniformLocation (GLuint program, const GLchar *name)
{
  return __glGetUniformLocation (program, name);
}

typedef GLuint (*PFNglCreateProgramPROC)(void);
fz_global PFNglCreateProgramPROC __glCreateProgram = NULL;
fz_function inline GLuint
glCreateProgram (void)
{
  return __glCreateProgram ();
}

typedef GLenum (*PFNglCheckNamedFramebufferStatusPROC)(GLuint framebuffer, GLenum target);
fz_global PFNglCheckNamedFramebufferStatusPROC __glCheckNamedFramebufferStatus = NULL;
fz_function inline GLenum
glCheckNamedFramebufferStatus (GLuint framebuffer, GLenum target)
{
  return __glCheckNamedFramebufferStatus (framebuffer, target);
}

typedef const GLubyte * (*PFNglGetStringPROC)(GLenum name);
fz_global PFNglGetStringPROC __glGetString = NULL;
fz_function inline const GLubyte *
glGetString (GLenum name)
{
  return __glGetString (name);
}

typedef void* (*PFNglMapNamedBufferRangePROC)(GLuint buffer, GLintptr offset, GLsizeiptr length, GLbitfield access);
fz_global PFNglMapNamedBufferRangePROC __glMapNamedBufferRange = NULL;
fz_function inline void*
glMapNamedBufferRange (GLuint buffer, GLintptr offset, GLsizeiptr length, GLbitfield access)
{
  return __glMapNamedBufferRange (buffer, offset, length, access);
}

typedef GLenum (*PFNglGetErrorPROC)(void);
fz_global PFNglGetErrorPROC __glGetError = NULL;
fz_function inline GLenum
glGetError (void)
{
  return __glGetError ();
}

typedef GLenum (*PFNglCheckFramebufferStatusPROC)(GLenum target);
fz_global PFNglCheckFramebufferStatusPROC __glCheckFramebufferStatus = NULL;
fz_function inline GLenum
glCheckFramebufferStatus (GLenum target)
{
  return __glCheckFramebufferStatus (target);
}

#endif // OPENGL_FUNCTIONS_H