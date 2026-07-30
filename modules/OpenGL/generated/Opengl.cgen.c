/* Generated code */

fz_internal b32
_opengl_load_functions()
{
  __glCreateShaderProgramv = (PFNglCreateShaderProgramvPROC)_load_gl_function("glCreateShaderProgramv"); 
  if (!glCreateShaderProgramv)
  {
    message_box(S("OpenGL"), S("Unable to load opengl fz_internal: glCreateShaderProgramv"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glGetUniformLocation = (PFNglGetUniformLocationPROC)_load_gl_function("glGetUniformLocation"); 
  if (!glGetUniformLocation)
  {
    message_box(S("OpenGL"), S("Unable to load opengl fz_internal: glGetUniformLocation"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glCreateProgram = (PFNglCreateProgramPROC)_load_gl_function("glCreateProgram"); 
  if (!glCreateProgram)
  {
    message_box(S("OpenGL"), S("Unable to load opengl fz_internal: glCreateProgram"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glCheckNamedFramebufferStatus = (PFNglCheckNamedFramebufferStatusPROC)_load_gl_function("glCheckNamedFramebufferStatus"); 
  if (!glCheckNamedFramebufferStatus)
  {
    message_box(S("OpenGL"), S("Unable to load opengl fz_internal: glCheckNamedFramebufferStatus"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glGetString = (PFNglGetStringPROC)_load_gl_function("glGetString"); 
  if (!glGetString)
  {
    message_box(S("OpenGL"), S("Unable to load opengl fz_internal: glGetString"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glMapNamedBufferRange = (PFNglMapNamedBufferRangePROC)_load_gl_function("glMapNamedBufferRange"); 
  if (!glMapNamedBufferRange)
  {
    message_box(S("OpenGL"), S("Unable to load opengl fz_internal: glMapNamedBufferRange"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glGetError = (PFNglGetErrorPROC)_load_gl_function("glGetError"); 
  if (!glGetError)
  {
    message_box(S("OpenGL"), S("Unable to load opengl fz_internal: glGetError"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glCheckFramebufferStatus = (PFNglCheckFramebufferStatusPROC)_load_gl_function("glCheckFramebufferStatus"); 
  if (!glCheckFramebufferStatus)
  {
    message_box(S("OpenGL"), S("Unable to load opengl fz_internal: glCheckFramebufferStatus"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glProgramUniform1i = (PFNglProgramUniform1iPROC)_load_gl_function("glProgramUniform1i"); 
  if (!__glProgramUniform1i)
  {
    message_box(S("OpenGL"), S("Unable to load opengl fz_internal: glProgramUniform1i"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glProgramUniform1f = (PFNglProgramUniform1fPROC)_load_gl_function("glProgramUniform1f"); 
  if (!__glProgramUniform1f)
  {
    message_box(S("OpenGL"), S("Unable to load opengl fz_internal: glProgramUniform1f"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glProgramUniform2f = (PFNglProgramUniform2fPROC)_load_gl_function("glProgramUniform2f"); 
  if (!__glProgramUniform2f)
  {
    message_box(S("OpenGL"), S("Unable to load opengl fz_internal: glProgramUniform2f"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glProgramUniform3f = (PFNglProgramUniform3fPROC)_load_gl_function("glProgramUniform3f"); 
  if (!__glProgramUniform3f)
  {
    message_box(S("OpenGL"), S("Unable to load opengl fz_internal: glProgramUniform3f"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glProgramUniform4f = (PFNglProgramUniform4fPROC)_load_gl_function("glProgramUniform4f"); 
  if (!__glProgramUniform4f)
  {
    message_box(S("OpenGL"), S("Unable to load opengl fz_internal: glProgramUniform4f"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glProgramUniform3fv = (PFNglProgramUniform3fvPROC)_load_gl_function("glProgramUniform3fv"); 
  if (!__glProgramUniform3fv)
  {
    message_box(S("OpenGL"), S("Unable to load opengl fz_internal: glProgramUniform3fv"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glProgramUniformMatrix4fv = (PFNglProgramUniformMatrix4fvPROC)_load_gl_function("glProgramUniformMatrix4fv"); 
  if (!__glProgramUniformMatrix4fv)
  {
    message_box(S("OpenGL"), S("Unable to load opengl fz_internal: glProgramUniformMatrix4fv"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glProgramUniformMatrix2fv = (PFNglProgramUniformMatrix2fvPROC)_load_gl_function("glProgramUniformMatrix2fv"); 
  if (!__glProgramUniformMatrix2fv)
  {
    message_box(S("OpenGL"), S("Unable to load opengl fz_internal: glProgramUniformMatrix2fv"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glAttachShader = (PFNglAttachShaderPROC)_load_gl_function("glAttachShader"); 
  if (!__glAttachShader)
  {
    message_box(S("OpenGL"), S("Unable to load opengl fz_internal: glAttachShader"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glLinkProgram = (PFNglLinkProgramPROC)_load_gl_function("glLinkProgram"); 
  if (!__glLinkProgram)
  {
    message_box(S("OpenGL"), S("Unable to load opengl fz_internal: glLinkProgram"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glDeleteShader = (PFNglDeleteShaderPROC)_load_gl_function("glDeleteShader"); 
  if (!__glDeleteShader)
  {
    message_box(S("OpenGL"), S("Unable to load opengl fz_internal: glDeleteShader"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glDeleteProgram = (PFNglDeleteProgramPROC)_load_gl_function("glDeleteProgram"); 
  if (!__glDeleteProgram)
  {
    message_box(S("OpenGL"), S("Unable to load opengl fz_internal: glDeleteProgram"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glGetProgramiv = (PFNglGetProgramivPROC)_load_gl_function("glGetProgramiv"); 
  if (!__glGetProgramiv)
  {
    message_box(S("OpenGL"), S("Unable to load opengl fz_internal: glGetProgramiv"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glGetProgramInfoLog = (PFNglGetProgramInfoLogPROC)_load_gl_function("glGetProgramInfoLog"); 
  if (!__glGetProgramInfoLog)
  {
    message_box(S("OpenGL"), S("Unable to load opengl fz_internal: glGetProgramInfoLog"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glGenProgramPipelines = (PFNglGenProgramPipelinesPROC)_load_gl_function("glGenProgramPipelines"); 
  if (!__glGenProgramPipelines)
  {
    message_box(S("OpenGL"), S("Unable to load opengl fz_internal: glGenProgramPipelines"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glBindProgramPipeline = (PFNglBindProgramPipelinePROC)_load_gl_function("glBindProgramPipeline"); 
  if (!__glBindProgramPipeline)
  {
    message_box(S("OpenGL"), S("Unable to load opengl fz_internal: glBindProgramPipeline"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glUseProgramStages = (PFNglUseProgramStagesPROC)_load_gl_function("glUseProgramStages"); 
  if (!__glUseProgramStages)
  {
    message_box(S("OpenGL"), S("Unable to load opengl fz_internal: glUseProgramStages"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glDeleteProgramPipelines = (PFNglDeleteProgramPipelinesPROC)_load_gl_function("glDeleteProgramPipelines"); 
  if (!__glDeleteProgramPipelines)
  {
    message_box(S("OpenGL"), S("Unable to load opengl fz_internal: glDeleteProgramPipelines"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glCreateBuffers = (PFNglCreateBuffersPROC)_load_gl_function("glCreateBuffers"); 
  if (!__glCreateBuffers)
  {
    message_box(S("OpenGL"), S("Unable to load opengl fz_internal: glCreateBuffers"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glNamedBufferData = (PFNglNamedBufferDataPROC)_load_gl_function("glNamedBufferData"); 
  if (!__glNamedBufferData)
  {
    message_box(S("OpenGL"), S("Unable to load opengl fz_internal: glNamedBufferData"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glNamedBufferSubData = (PFNglNamedBufferSubDataPROC)_load_gl_function("glNamedBufferSubData"); 
  if (!__glNamedBufferSubData)
  {
    message_box(S("OpenGL"), S("Unable to load opengl fz_internal: glNamedBufferSubData"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glDeleteBuffers = (PFNglDeleteBuffersPROC)_load_gl_function("glDeleteBuffers"); 
  if (!__glDeleteBuffers)
  {
    message_box(S("OpenGL"), S("Unable to load opengl fz_internal: glDeleteBuffers"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glBindBuffer = (PFNglBindBufferPROC)_load_gl_function("glBindBuffer"); 
  if (!__glBindBuffer)
  {
    message_box(S("OpenGL"), S("Unable to load opengl fz_internal: glBindBuffer"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glNamedBufferStorage = (PFNglNamedBufferStoragePROC)_load_gl_function("glNamedBufferStorage"); 
  if (!__glNamedBufferStorage)
  {
    message_box(S("OpenGL"), S("Unable to load opengl fz_internal: glNamedBufferStorage"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glCreateProgramPipelines = (PFNglCreateProgramPipelinesPROC)_load_gl_function("glCreateProgramPipelines"); 
  if (!__glCreateProgramPipelines)
  {
    message_box(S("OpenGL"), S("Unable to load opengl fz_internal: glCreateProgramPipelines"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glCreateVertexArrays = (PFNglCreateVertexArraysPROC)_load_gl_function("glCreateVertexArrays"); 
  if (!__glCreateVertexArrays)
  {
    message_box(S("OpenGL"), S("Unable to load opengl fz_internal: glCreateVertexArrays"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glVertexArrayVertexBuffer = (PFNglVertexArrayVertexBufferPROC)_load_gl_function("glVertexArrayVertexBuffer"); 
  if (!__glVertexArrayVertexBuffer)
  {
    message_box(S("OpenGL"), S("Unable to load opengl fz_internal: glVertexArrayVertexBuffer"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glEnableVertexArrayAttrib = (PFNglEnableVertexArrayAttribPROC)_load_gl_function("glEnableVertexArrayAttrib"); 
  if (!__glEnableVertexArrayAttrib)
  {
    message_box(S("OpenGL"), S("Unable to load opengl fz_internal: glEnableVertexArrayAttrib"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glVertexArrayAttribFormat = (PFNglVertexArrayAttribFormatPROC)_load_gl_function("glVertexArrayAttribFormat"); 
  if (!__glVertexArrayAttribFormat)
  {
    message_box(S("OpenGL"), S("Unable to load opengl fz_internal: glVertexArrayAttribFormat"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glVertexArrayAttribBinding = (PFNglVertexArrayAttribBindingPROC)_load_gl_function("glVertexArrayAttribBinding"); 
  if (!__glVertexArrayAttribBinding)
  {
    message_box(S("OpenGL"), S("Unable to load opengl fz_internal: glVertexArrayAttribBinding"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glBindVertexArray = (PFNglBindVertexArrayPROC)_load_gl_function("glBindVertexArray"); 
  if (!__glBindVertexArray)
  {
    message_box(S("OpenGL"), S("Unable to load opengl fz_internal: glBindVertexArray"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glDeleteVertexArrays = (PFNglDeleteVertexArraysPROC)_load_gl_function("glDeleteVertexArrays"); 
  if (!__glDeleteVertexArrays)
  {
    message_box(S("OpenGL"), S("Unable to load opengl fz_internal: glDeleteVertexArrays"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glVertexArrayBindingDivisor = (PFNglVertexArrayBindingDivisorPROC)_load_gl_function("glVertexArrayBindingDivisor"); 
  if (!__glVertexArrayBindingDivisor)
  {
    message_box(S("OpenGL"), S("Unable to load opengl fz_internal: glVertexArrayBindingDivisor"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glCreateTextures = (PFNglCreateTexturesPROC)_load_gl_function("glCreateTextures"); 
  if (!__glCreateTextures)
  {
    message_box(S("OpenGL"), S("Unable to load opengl fz_internal: glCreateTextures"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glTextureStorage2D = (PFNglTextureStorage2DPROC)_load_gl_function("glTextureStorage2D"); 
  if (!__glTextureStorage2D)
  {
    message_box(S("OpenGL"), S("Unable to load opengl fz_internal: glTextureStorage2D"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glTextureSubImage2D = (PFNglTextureSubImage2DPROC)_load_gl_function("glTextureSubImage2D"); 
  if (!__glTextureSubImage2D)
  {
    message_box(S("OpenGL"), S("Unable to load opengl fz_internal: glTextureSubImage2D"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glTextureParameteri = (PFNglTextureParameteriPROC)_load_gl_function("glTextureParameteri"); 
  if (!__glTextureParameteri)
  {
    message_box(S("OpenGL"), S("Unable to load opengl fz_internal: glTextureParameteri"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glBindTextureUnit = (PFNglBindTextureUnitPROC)_load_gl_function("glBindTextureUnit"); 
  if (!__glBindTextureUnit)
  {
    message_box(S("OpenGL"), S("Unable to load opengl fz_internal: glBindTextureUnit"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glDeleteTextures = (PFNglDeleteTexturesPROC)_load_gl_function("glDeleteTextures"); 
  if (!__glDeleteTextures)
  {
    message_box(S("OpenGL"), S("Unable to load opengl fz_internal: glDeleteTextures"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glCreateFramebuffers = (PFNglCreateFramebuffersPROC)_load_gl_function("glCreateFramebuffers"); 
  if (!__glCreateFramebuffers)
  {
    message_box(S("OpenGL"), S("Unable to load opengl fz_internal: glCreateFramebuffers"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glNamedFramebufferTexture = (PFNglNamedFramebufferTexturePROC)_load_gl_function("glNamedFramebufferTexture"); 
  if (!__glNamedFramebufferTexture)
  {
    message_box(S("OpenGL"), S("Unable to load opengl fz_internal: glNamedFramebufferTexture"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glNamedFramebufferRenderbuffer = (PFNglNamedFramebufferRenderbufferPROC)_load_gl_function("glNamedFramebufferRenderbuffer"); 
  if (!__glNamedFramebufferRenderbuffer)
  {
    message_box(S("OpenGL"), S("Unable to load opengl fz_internal: glNamedFramebufferRenderbuffer"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glDeleteFramebuffers = (PFNglDeleteFramebuffersPROC)_load_gl_function("glDeleteFramebuffers"); 
  if (!__glDeleteFramebuffers)
  {
    message_box(S("OpenGL"), S("Unable to load opengl fz_internal: glDeleteFramebuffers"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glBindFramebuffer = (PFNglBindFramebufferPROC)_load_gl_function("glBindFramebuffer"); 
  if (!__glBindFramebuffer)
  {
    message_box(S("OpenGL"), S("Unable to load opengl fz_internal: glBindFramebuffer"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glDrawArrays = (PFNglDrawArraysPROC)_load_gl_function("glDrawArrays"); 
  if (!__glDrawArrays)
  {
    message_box(S("OpenGL"), S("Unable to load opengl fz_internal: glDrawArrays"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glDrawElements = (PFNglDrawElementsPROC)_load_gl_function("glDrawElements"); 
  if (!__glDrawElements)
  {
    message_box(S("OpenGL"), S("Unable to load opengl fz_internal: glDrawElements"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glDrawArraysInstanced = (PFNglDrawArraysInstancedPROC)_load_gl_function("glDrawArraysInstanced"); 
  if (!__glDrawArraysInstanced)
  {
    message_box(S("OpenGL"), S("Unable to load opengl fz_internal: glDrawArraysInstanced"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glDrawElementsInstanced = (PFNglDrawElementsInstancedPROC)_load_gl_function("glDrawElementsInstanced"); 
  if (!__glDrawElementsInstanced)
  {
    message_box(S("OpenGL"), S("Unable to load opengl fz_internal: glDrawElementsInstanced"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glClearColor = (PFNglClearColorPROC)_load_gl_function("glClearColor"); 
  if (!__glClearColor)
  {
    message_box(S("OpenGL"), S("Unable to load opengl fz_internal: glClearColor"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glDebugMessageCallback = (PFNglDebugMessageCallbackPROC)_load_gl_function("glDebugMessageCallback"); 
  if (!__glDebugMessageCallback)
  {
    message_box(S("OpenGL"), S("Unable to load opengl fz_internal: glDebugMessageCallback"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glEnable = (PFNglEnablePROC)_load_gl_function("glEnable"); 
  if (!__glEnable)
  {
    message_box(S("OpenGL"), S("Unable to load opengl fz_internal: glEnable"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glGetIntegerv = (PFNglGetIntegervPROC)_load_gl_function("glGetIntegerv"); 
  if (!__glGetIntegerv)
  {
    message_box(S("OpenGL"), S("Unable to load opengl fz_internal: glGetIntegerv"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glVertexArrayAttribIFormat = (PFNglVertexArrayAttribIFormatPROC)_load_gl_function("glVertexArrayAttribIFormat"); 
  if (!__glVertexArrayAttribIFormat)
  {
    message_box(S("OpenGL"), S("Unable to load opengl fz_internal: glVertexArrayAttribIFormat"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glUniform1iv = (PFNglUniform1ivPROC)_load_gl_function("glUniform1iv"); 
  if (!__glUniform1iv)
  {
    message_box(S("OpenGL"), S("Unable to load opengl fz_internal: glUniform1iv"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glUseProgram = (PFNglUseProgramPROC)_load_gl_function("glUseProgram"); 
  if (!__glUseProgram)
  {
    message_box(S("OpenGL"), S("Unable to load opengl fz_internal: glUseProgram"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glProgramUniform1iv = (PFNglProgramUniform1ivPROC)_load_gl_function("glProgramUniform1iv"); 
  if (!__glProgramUniform1iv)
  {
    message_box(S("OpenGL"), S("Unable to load opengl fz_internal: glProgramUniform1iv"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glPolygonMode = (PFNglPolygonModePROC)_load_gl_function("glPolygonMode"); 
  if (!__glPolygonMode)
  {
    message_box(S("OpenGL"), S("Unable to load opengl fz_internal: glPolygonMode"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glDepthRangef = (PFNglDepthRangefPROC)_load_gl_function("glDepthRangef"); 
  if (!__glDepthRangef)
  {
    message_box(S("OpenGL"), S("Unable to load opengl fz_internal: glDepthRangef"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glVertexArrayElementBuffer = (PFNglVertexArrayElementBufferPROC)_load_gl_function("glVertexArrayElementBuffer"); 
  if (!__glVertexArrayElementBuffer)
  {
    message_box(S("OpenGL"), S("Unable to load opengl fz_internal: glVertexArrayElementBuffer"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glProgramUniformMatrix3fv = (PFNglProgramUniformMatrix3fvPROC)_load_gl_function("glProgramUniformMatrix3fv"); 
  if (!__glProgramUniformMatrix3fv)
  {
    message_box(S("OpenGL"), S("Unable to load opengl fz_internal: glProgramUniformMatrix3fv"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glViewport = (PFNglViewportPROC)_load_gl_function("glViewport"); 
  if (!__glViewport)
  {
    message_box(S("OpenGL"), S("Unable to load opengl fz_internal: glViewport"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glBlendFunc = (PFNglBlendFuncPROC)_load_gl_function("glBlendFunc"); 
  if (!__glBlendFunc)
  {
    message_box(S("OpenGL"), S("Unable to load opengl fz_internal: glBlendFunc"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glClear = (PFNglClearPROC)_load_gl_function("glClear"); 
  if (!__glClear)
  {
    message_box(S("OpenGL"), S("Unable to load opengl fz_internal: glClear"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glValidateProgramPipeline = (PFNglValidateProgramPipelinePROC)_load_gl_function("glValidateProgramPipeline"); 
  if (!__glValidateProgramPipeline)
  {
    message_box(S("OpenGL"), S("Unable to load opengl fz_internal: glValidateProgramPipeline"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glGetProgramPipelineiv = (PFNglGetProgramPipelineivPROC)_load_gl_function("glGetProgramPipelineiv"); 
  if (!__glGetProgramPipelineiv)
  {
    message_box(S("OpenGL"), S("Unable to load opengl fz_internal: glGetProgramPipelineiv"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glGetProgramPipelineInfoLog = (PFNglGetProgramPipelineInfoLogPROC)_load_gl_function("glGetProgramPipelineInfoLog"); 
  if (!__glGetProgramPipelineInfoLog)
  {
    message_box(S("OpenGL"), S("Unable to load opengl fz_internal: glGetProgramPipelineInfoLog"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glDepthMask = (PFNglDepthMaskPROC)_load_gl_function("glDepthMask"); 
  if (!__glDepthMask)
  {
    message_box(S("OpenGL"), S("Unable to load opengl fz_internal: glDepthMask"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glUniformMatrix4fv = (PFNglUniformMatrix4fvPROC)_load_gl_function("glUniformMatrix4fv"); 
  if (!__glUniformMatrix4fv)
  {
    message_box(S("OpenGL"), S("Unable to load opengl fz_internal: glUniformMatrix4fv"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glUniform1i = (PFNglUniform1iPROC)_load_gl_function("glUniform1i"); 
  if (!__glUniform1i)
  {
    message_box(S("OpenGL"), S("Unable to load opengl fz_internal: glUniform1i"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glDepthFunc = (PFNglDepthFuncPROC)_load_gl_function("glDepthFunc"); 
  if (!__glDepthFunc)
  {
    message_box(S("OpenGL"), S("Unable to load opengl fz_internal: glDepthFunc"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glGenTextures = (PFNglGenTexturesPROC)_load_gl_function("glGenTextures"); 
  if (!__glGenTextures)
  {
    message_box(S("OpenGL"), S("Unable to load opengl fz_internal: glGenTextures"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glBindTexture = (PFNglBindTexturePROC)_load_gl_function("glBindTexture"); 
  if (!__glBindTexture)
  {
    message_box(S("OpenGL"), S("Unable to load opengl fz_internal: glBindTexture"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glTexImage2D = (PFNglTexImage2DPROC)_load_gl_function("glTexImage2D"); 
  if (!__glTexImage2D)
  {
    message_box(S("OpenGL"), S("Unable to load opengl fz_internal: glTexImage2D"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glTexParameteri = (PFNglTexParameteriPROC)_load_gl_function("glTexParameteri"); 
  if (!__glTexParameteri)
  {
    message_box(S("OpenGL"), S("Unable to load opengl fz_internal: glTexParameteri"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glPixelStorei = (PFNglPixelStoreiPROC)_load_gl_function("glPixelStorei"); 
  if (!__glPixelStorei)
  {
    message_box(S("OpenGL"), S("Unable to load opengl fz_internal: glPixelStorei"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glGenerateMipmap = (PFNglGenerateMipmapPROC)_load_gl_function("glGenerateMipmap"); 
  if (!__glGenerateMipmap)
  {
    message_box(S("OpenGL"), S("Unable to load opengl fz_internal: glGenerateMipmap"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glDisable = (PFNglDisablePROC)_load_gl_function("glDisable"); 
  if (!__glDisable)
  {
    message_box(S("OpenGL"), S("Unable to load opengl fz_internal: glDisable"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glGenFramebuffers = (PFNglGenFramebuffersPROC)_load_gl_function("glGenFramebuffers"); 
  if (!__glGenFramebuffers)
  {
    message_box(S("OpenGL"), S("Unable to load opengl fz_internal: glGenFramebuffers"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glFramebufferTexture2D = (PFNglFramebufferTexture2DPROC)_load_gl_function("glFramebufferTexture2D"); 
  if (!__glFramebufferTexture2D)
  {
    message_box(S("OpenGL"), S("Unable to load opengl fz_internal: glFramebufferTexture2D"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glGetTexLevelParameteriv = (PFNglGetTexLevelParameterivPROC)_load_gl_function("glGetTexLevelParameteriv"); 
  if (!__glGetTexLevelParameteriv)
  {
    message_box(S("OpenGL"), S("Unable to load opengl fz_internal: glGetTexLevelParameteriv"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glReadBuffer = (PFNglReadBufferPROC)_load_gl_function("glReadBuffer"); 
  if (!__glReadBuffer)
  {
    message_box(S("OpenGL"), S("Unable to load opengl fz_internal: glReadBuffer"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glReadPixels = (PFNglReadPixelsPROC)_load_gl_function("glReadPixels"); 
  if (!__glReadPixels)
  {
    message_box(S("OpenGL"), S("Unable to load opengl fz_internal: glReadPixels"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glGenQueries = (PFNglGenQueriesPROC)_load_gl_function("glGenQueries"); 
  if (!__glGenQueries)
  {
    message_box(S("OpenGL"), S("Unable to load opengl fz_internal: glGenQueries"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glGetQueryObjectui64v = (PFNglGetQueryObjectui64vPROC)_load_gl_function("glGetQueryObjectui64v"); 
  if (!__glGetQueryObjectui64v)
  {
    message_box(S("OpenGL"), S("Unable to load opengl fz_internal: glGetQueryObjectui64v"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glBeginQuery = (PFNglBeginQueryPROC)_load_gl_function("glBeginQuery"); 
  if (!__glBeginQuery)
  {
    message_box(S("OpenGL"), S("Unable to load opengl fz_internal: glBeginQuery"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glEndQuery = (PFNglEndQueryPROC)_load_gl_function("glEndQuery"); 
  if (!__glEndQuery)
  {
    message_box(S("OpenGL"), S("Unable to load opengl fz_internal: glEndQuery"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glBlendFuncSeparate = (PFNglBlendFuncSeparatePROC)_load_gl_function("glBlendFuncSeparate"); 
  if (!__glBlendFuncSeparate)
  {
    message_box(S("OpenGL"), S("Unable to load opengl fz_internal: glBlendFuncSeparate"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glFinish = (PFNglFinishPROC)_load_gl_function("glFinish"); 
  if (!__glFinish)
  {
    message_box(S("OpenGL"), S("Unable to load opengl fz_internal: glFinish"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glObjectLabel = (PFNglObjectLabelPROC)_load_gl_function("glObjectLabel"); 
  if (!__glObjectLabel)
  {
    message_box(S("OpenGL"), S("Unable to load opengl fz_internal: glObjectLabel"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glPushDebugGroup = (PFNglPushDebugGroupPROC)_load_gl_function("glPushDebugGroup"); 
  if (!__glPushDebugGroup)
  {
    message_box(S("OpenGL"), S("Unable to load opengl fz_internal: glPushDebugGroup"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glPopDebugGroup = (PFNglPopDebugGroupPROC)_load_gl_function("glPopDebugGroup"); 
  if (!__glPopDebugGroup)
  {
    message_box(S("OpenGL"), S("Unable to load opengl fz_internal: glPopDebugGroup"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glPointSize = (PFNglPointSizePROC)_load_gl_function("glPointSize"); 
  if (!__glPointSize)
  {
    message_box(S("OpenGL"), S("Unable to load opengl fz_internal: glPointSize"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glLineWidth = (PFNglLineWidthPROC)_load_gl_function("glLineWidth"); 
  if (!__glLineWidth)
  {
    message_box(S("OpenGL"), S("Unable to load opengl fz_internal: glLineWidth"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glClearTexImage = (PFNglClearTexImagePROC)_load_gl_function("glClearTexImage"); 
  if (!__glClearTexImage)
  {
    message_box(S("OpenGL"), S("Unable to load opengl fz_internal: glClearTexImage"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glBindBufferBase = (PFNglBindBufferBasePROC)_load_gl_function("glBindBufferBase"); 
  if (!__glBindBufferBase)
  {
    message_box(S("OpenGL"), S("Unable to load opengl fz_internal: glBindBufferBase"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glTextureStorage3D = (PFNglTextureStorage3DPROC)_load_gl_function("glTextureStorage3D"); 
  if (!__glTextureStorage3D)
  {
    message_box(S("OpenGL"), S("Unable to load opengl fz_internal: glTextureStorage3D"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glTextureSubImage3D = (PFNglTextureSubImage3DPROC)_load_gl_function("glTextureSubImage3D"); 
  if (!__glTextureSubImage3D)
  {
    message_box(S("OpenGL"), S("Unable to load opengl fz_internal: glTextureSubImage3D"), S("OpenGL.cgen"), 0);
    return false;
  }

  return true;
}