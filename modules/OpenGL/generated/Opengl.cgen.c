/* Generated code */

function b32
_opengl_load_functions()
{
  __glCreateShaderProgramv = (PFNglCreateShaderProgramvPROC)_load_gl_function("glCreateShaderProgramv"); 
  if (!glCreateShaderProgramv)
  {
    message_box(S("OpenGL"), S("Unable to load opengl function: glCreateShaderProgramv"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glGetUniformLocation = (PFNglGetUniformLocationPROC)_load_gl_function("glGetUniformLocation"); 
  if (!glGetUniformLocation)
  {
    message_box(S("OpenGL"), S("Unable to load opengl function: glGetUniformLocation"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glCreateProgram = (PFNglCreateProgramPROC)_load_gl_function("glCreateProgram"); 
  if (!glCreateProgram)
  {
    message_box(S("OpenGL"), S("Unable to load opengl function: glCreateProgram"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glCheckNamedFramebufferStatus = (PFNglCheckNamedFramebufferStatusPROC)_load_gl_function("glCheckNamedFramebufferStatus"); 
  if (!glCheckNamedFramebufferStatus)
  {
    message_box(S("OpenGL"), S("Unable to load opengl function: glCheckNamedFramebufferStatus"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glGetString = (PFNglGetStringPROC)_load_gl_function("glGetString"); 
  if (!glGetString)
  {
    message_box(S("OpenGL"), S("Unable to load opengl function: glGetString"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glMapNamedBufferRange = (PFNglMapNamedBufferRangePROC)_load_gl_function("glMapNamedBufferRange"); 
  if (!glMapNamedBufferRange)
  {
    message_box(S("OpenGL"), S("Unable to load opengl function: glMapNamedBufferRange"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glGetError = (PFNglGetErrorPROC)_load_gl_function("glGetError"); 
  if (!glGetError)
  {
    message_box(S("OpenGL"), S("Unable to load opengl function: glGetError"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glCheckFramebufferStatus = (PFNglCheckFramebufferStatusPROC)_load_gl_function("glCheckFramebufferStatus"); 
  if (!glCheckFramebufferStatus)
  {
    message_box(S("OpenGL"), S("Unable to load opengl function: glCheckFramebufferStatus"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glProgramUniform1i = (PFNglProgramUniform1iPROC)_load_gl_function("glProgramUniform1i"); 
  if (!__glProgramUniform1i)
  {
    message_box(S("OpenGL"), S("Unable to load opengl function: glProgramUniform1i"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glProgramUniform1f = (PFNglProgramUniform1fPROC)_load_gl_function("glProgramUniform1f"); 
  if (!__glProgramUniform1f)
  {
    message_box(S("OpenGL"), S("Unable to load opengl function: glProgramUniform1f"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glProgramUniform2f = (PFNglProgramUniform2fPROC)_load_gl_function("glProgramUniform2f"); 
  if (!__glProgramUniform2f)
  {
    message_box(S("OpenGL"), S("Unable to load opengl function: glProgramUniform2f"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glProgramUniform3f = (PFNglProgramUniform3fPROC)_load_gl_function("glProgramUniform3f"); 
  if (!__glProgramUniform3f)
  {
    message_box(S("OpenGL"), S("Unable to load opengl function: glProgramUniform3f"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glProgramUniform4f = (PFNglProgramUniform4fPROC)_load_gl_function("glProgramUniform4f"); 
  if (!__glProgramUniform4f)
  {
    message_box(S("OpenGL"), S("Unable to load opengl function: glProgramUniform4f"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glProgramUniformMatrix4fv = (PFNglProgramUniformMatrix4fvPROC)_load_gl_function("glProgramUniformMatrix4fv"); 
  if (!__glProgramUniformMatrix4fv)
  {
    message_box(S("OpenGL"), S("Unable to load opengl function: glProgramUniformMatrix4fv"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glProgramUniformMatrix2fv = (PFNglProgramUniformMatrix2fvPROC)_load_gl_function("glProgramUniformMatrix2fv"); 
  if (!__glProgramUniformMatrix2fv)
  {
    message_box(S("OpenGL"), S("Unable to load opengl function: glProgramUniformMatrix2fv"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glAttachShader = (PFNglAttachShaderPROC)_load_gl_function("glAttachShader"); 
  if (!__glAttachShader)
  {
    message_box(S("OpenGL"), S("Unable to load opengl function: glAttachShader"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glLinkProgram = (PFNglLinkProgramPROC)_load_gl_function("glLinkProgram"); 
  if (!__glLinkProgram)
  {
    message_box(S("OpenGL"), S("Unable to load opengl function: glLinkProgram"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glDeleteShader = (PFNglDeleteShaderPROC)_load_gl_function("glDeleteShader"); 
  if (!__glDeleteShader)
  {
    message_box(S("OpenGL"), S("Unable to load opengl function: glDeleteShader"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glDeleteProgram = (PFNglDeleteProgramPROC)_load_gl_function("glDeleteProgram"); 
  if (!__glDeleteProgram)
  {
    message_box(S("OpenGL"), S("Unable to load opengl function: glDeleteProgram"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glGetProgramiv = (PFNglGetProgramivPROC)_load_gl_function("glGetProgramiv"); 
  if (!__glGetProgramiv)
  {
    message_box(S("OpenGL"), S("Unable to load opengl function: glGetProgramiv"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glGetProgramInfoLog = (PFNglGetProgramInfoLogPROC)_load_gl_function("glGetProgramInfoLog"); 
  if (!__glGetProgramInfoLog)
  {
    message_box(S("OpenGL"), S("Unable to load opengl function: glGetProgramInfoLog"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glGenProgramPipelines = (PFNglGenProgramPipelinesPROC)_load_gl_function("glGenProgramPipelines"); 
  if (!__glGenProgramPipelines)
  {
    message_box(S("OpenGL"), S("Unable to load opengl function: glGenProgramPipelines"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glBindProgramPipeline = (PFNglBindProgramPipelinePROC)_load_gl_function("glBindProgramPipeline"); 
  if (!__glBindProgramPipeline)
  {
    message_box(S("OpenGL"), S("Unable to load opengl function: glBindProgramPipeline"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glUseProgramStages = (PFNglUseProgramStagesPROC)_load_gl_function("glUseProgramStages"); 
  if (!__glUseProgramStages)
  {
    message_box(S("OpenGL"), S("Unable to load opengl function: glUseProgramStages"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glDeleteProgramPipelines = (PFNglDeleteProgramPipelinesPROC)_load_gl_function("glDeleteProgramPipelines"); 
  if (!__glDeleteProgramPipelines)
  {
    message_box(S("OpenGL"), S("Unable to load opengl function: glDeleteProgramPipelines"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glCreateBuffers = (PFNglCreateBuffersPROC)_load_gl_function("glCreateBuffers"); 
  if (!__glCreateBuffers)
  {
    message_box(S("OpenGL"), S("Unable to load opengl function: glCreateBuffers"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glNamedBufferData = (PFNglNamedBufferDataPROC)_load_gl_function("glNamedBufferData"); 
  if (!__glNamedBufferData)
  {
    message_box(S("OpenGL"), S("Unable to load opengl function: glNamedBufferData"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glNamedBufferSubData = (PFNglNamedBufferSubDataPROC)_load_gl_function("glNamedBufferSubData"); 
  if (!__glNamedBufferSubData)
  {
    message_box(S("OpenGL"), S("Unable to load opengl function: glNamedBufferSubData"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glDeleteBuffers = (PFNglDeleteBuffersPROC)_load_gl_function("glDeleteBuffers"); 
  if (!__glDeleteBuffers)
  {
    message_box(S("OpenGL"), S("Unable to load opengl function: glDeleteBuffers"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glBindBuffer = (PFNglBindBufferPROC)_load_gl_function("glBindBuffer"); 
  if (!__glBindBuffer)
  {
    message_box(S("OpenGL"), S("Unable to load opengl function: glBindBuffer"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glNamedBufferStorage = (PFNglNamedBufferStoragePROC)_load_gl_function("glNamedBufferStorage"); 
  if (!__glNamedBufferStorage)
  {
    message_box(S("OpenGL"), S("Unable to load opengl function: glNamedBufferStorage"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glCreateProgramPipelines = (PFNglCreateProgramPipelinesPROC)_load_gl_function("glCreateProgramPipelines"); 
  if (!__glCreateProgramPipelines)
  {
    message_box(S("OpenGL"), S("Unable to load opengl function: glCreateProgramPipelines"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glCreateVertexArrays = (PFNglCreateVertexArraysPROC)_load_gl_function("glCreateVertexArrays"); 
  if (!__glCreateVertexArrays)
  {
    message_box(S("OpenGL"), S("Unable to load opengl function: glCreateVertexArrays"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glVertexArrayVertexBuffer = (PFNglVertexArrayVertexBufferPROC)_load_gl_function("glVertexArrayVertexBuffer"); 
  if (!__glVertexArrayVertexBuffer)
  {
    message_box(S("OpenGL"), S("Unable to load opengl function: glVertexArrayVertexBuffer"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glEnableVertexArrayAttrib = (PFNglEnableVertexArrayAttribPROC)_load_gl_function("glEnableVertexArrayAttrib"); 
  if (!__glEnableVertexArrayAttrib)
  {
    message_box(S("OpenGL"), S("Unable to load opengl function: glEnableVertexArrayAttrib"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glVertexArrayAttribFormat = (PFNglVertexArrayAttribFormatPROC)_load_gl_function("glVertexArrayAttribFormat"); 
  if (!__glVertexArrayAttribFormat)
  {
    message_box(S("OpenGL"), S("Unable to load opengl function: glVertexArrayAttribFormat"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glVertexArrayAttribBinding = (PFNglVertexArrayAttribBindingPROC)_load_gl_function("glVertexArrayAttribBinding"); 
  if (!__glVertexArrayAttribBinding)
  {
    message_box(S("OpenGL"), S("Unable to load opengl function: glVertexArrayAttribBinding"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glBindVertexArray = (PFNglBindVertexArrayPROC)_load_gl_function("glBindVertexArray"); 
  if (!__glBindVertexArray)
  {
    message_box(S("OpenGL"), S("Unable to load opengl function: glBindVertexArray"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glDeleteVertexArrays = (PFNglDeleteVertexArraysPROC)_load_gl_function("glDeleteVertexArrays"); 
  if (!__glDeleteVertexArrays)
  {
    message_box(S("OpenGL"), S("Unable to load opengl function: glDeleteVertexArrays"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glVertexArrayBindingDivisor = (PFNglVertexArrayBindingDivisorPROC)_load_gl_function("glVertexArrayBindingDivisor"); 
  if (!__glVertexArrayBindingDivisor)
  {
    message_box(S("OpenGL"), S("Unable to load opengl function: glVertexArrayBindingDivisor"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glCreateTextures = (PFNglCreateTexturesPROC)_load_gl_function("glCreateTextures"); 
  if (!__glCreateTextures)
  {
    message_box(S("OpenGL"), S("Unable to load opengl function: glCreateTextures"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glTextureStorage2D = (PFNglTextureStorage2DPROC)_load_gl_function("glTextureStorage2D"); 
  if (!__glTextureStorage2D)
  {
    message_box(S("OpenGL"), S("Unable to load opengl function: glTextureStorage2D"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glTextureSubImage2D = (PFNglTextureSubImage2DPROC)_load_gl_function("glTextureSubImage2D"); 
  if (!__glTextureSubImage2D)
  {
    message_box(S("OpenGL"), S("Unable to load opengl function: glTextureSubImage2D"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glTextureParameteri = (PFNglTextureParameteriPROC)_load_gl_function("glTextureParameteri"); 
  if (!__glTextureParameteri)
  {
    message_box(S("OpenGL"), S("Unable to load opengl function: glTextureParameteri"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glBindTextureUnit = (PFNglBindTextureUnitPROC)_load_gl_function("glBindTextureUnit"); 
  if (!__glBindTextureUnit)
  {
    message_box(S("OpenGL"), S("Unable to load opengl function: glBindTextureUnit"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glDeleteTextures = (PFNglDeleteTexturesPROC)_load_gl_function("glDeleteTextures"); 
  if (!__glDeleteTextures)
  {
    message_box(S("OpenGL"), S("Unable to load opengl function: glDeleteTextures"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glCreateFramebuffers = (PFNglCreateFramebuffersPROC)_load_gl_function("glCreateFramebuffers"); 
  if (!__glCreateFramebuffers)
  {
    message_box(S("OpenGL"), S("Unable to load opengl function: glCreateFramebuffers"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glNamedFramebufferTexture = (PFNglNamedFramebufferTexturePROC)_load_gl_function("glNamedFramebufferTexture"); 
  if (!__glNamedFramebufferTexture)
  {
    message_box(S("OpenGL"), S("Unable to load opengl function: glNamedFramebufferTexture"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glNamedFramebufferRenderbuffer = (PFNglNamedFramebufferRenderbufferPROC)_load_gl_function("glNamedFramebufferRenderbuffer"); 
  if (!__glNamedFramebufferRenderbuffer)
  {
    message_box(S("OpenGL"), S("Unable to load opengl function: glNamedFramebufferRenderbuffer"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glDeleteFramebuffers = (PFNglDeleteFramebuffersPROC)_load_gl_function("glDeleteFramebuffers"); 
  if (!__glDeleteFramebuffers)
  {
    message_box(S("OpenGL"), S("Unable to load opengl function: glDeleteFramebuffers"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glBindFramebuffer = (PFNglBindFramebufferPROC)_load_gl_function("glBindFramebuffer"); 
  if (!__glBindFramebuffer)
  {
    message_box(S("OpenGL"), S("Unable to load opengl function: glBindFramebuffer"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glDrawArrays = (PFNglDrawArraysPROC)_load_gl_function("glDrawArrays"); 
  if (!__glDrawArrays)
  {
    message_box(S("OpenGL"), S("Unable to load opengl function: glDrawArrays"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glDrawElements = (PFNglDrawElementsPROC)_load_gl_function("glDrawElements"); 
  if (!__glDrawElements)
  {
    message_box(S("OpenGL"), S("Unable to load opengl function: glDrawElements"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glDrawArraysInstanced = (PFNglDrawArraysInstancedPROC)_load_gl_function("glDrawArraysInstanced"); 
  if (!__glDrawArraysInstanced)
  {
    message_box(S("OpenGL"), S("Unable to load opengl function: glDrawArraysInstanced"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glDrawElementsInstanced = (PFNglDrawElementsInstancedPROC)_load_gl_function("glDrawElementsInstanced"); 
  if (!__glDrawElementsInstanced)
  {
    message_box(S("OpenGL"), S("Unable to load opengl function: glDrawElementsInstanced"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glClearColor = (PFNglClearColorPROC)_load_gl_function("glClearColor"); 
  if (!__glClearColor)
  {
    message_box(S("OpenGL"), S("Unable to load opengl function: glClearColor"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glDebugMessageCallback = (PFNglDebugMessageCallbackPROC)_load_gl_function("glDebugMessageCallback"); 
  if (!__glDebugMessageCallback)
  {
    message_box(S("OpenGL"), S("Unable to load opengl function: glDebugMessageCallback"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glEnable = (PFNglEnablePROC)_load_gl_function("glEnable"); 
  if (!__glEnable)
  {
    message_box(S("OpenGL"), S("Unable to load opengl function: glEnable"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glGetIntegerv = (PFNglGetIntegervPROC)_load_gl_function("glGetIntegerv"); 
  if (!__glGetIntegerv)
  {
    message_box(S("OpenGL"), S("Unable to load opengl function: glGetIntegerv"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glVertexArrayAttribIFormat = (PFNglVertexArrayAttribIFormatPROC)_load_gl_function("glVertexArrayAttribIFormat"); 
  if (!__glVertexArrayAttribIFormat)
  {
    message_box(S("OpenGL"), S("Unable to load opengl function: glVertexArrayAttribIFormat"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glUniform1iv = (PFNglUniform1ivPROC)_load_gl_function("glUniform1iv"); 
  if (!__glUniform1iv)
  {
    message_box(S("OpenGL"), S("Unable to load opengl function: glUniform1iv"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glUseProgram = (PFNglUseProgramPROC)_load_gl_function("glUseProgram"); 
  if (!__glUseProgram)
  {
    message_box(S("OpenGL"), S("Unable to load opengl function: glUseProgram"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glProgramUniform1iv = (PFNglProgramUniform1ivPROC)_load_gl_function("glProgramUniform1iv"); 
  if (!__glProgramUniform1iv)
  {
    message_box(S("OpenGL"), S("Unable to load opengl function: glProgramUniform1iv"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glPolygonMode = (PFNglPolygonModePROC)_load_gl_function("glPolygonMode"); 
  if (!__glPolygonMode)
  {
    message_box(S("OpenGL"), S("Unable to load opengl function: glPolygonMode"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glDepthRangef = (PFNglDepthRangefPROC)_load_gl_function("glDepthRangef"); 
  if (!__glDepthRangef)
  {
    message_box(S("OpenGL"), S("Unable to load opengl function: glDepthRangef"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glLineWidth = (PFNglLineWidthPROC)_load_gl_function("glLineWidth"); 
  if (!__glLineWidth)
  {
    message_box(S("OpenGL"), S("Unable to load opengl function: glLineWidth"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glVertexArrayElementBuffer = (PFNglVertexArrayElementBufferPROC)_load_gl_function("glVertexArrayElementBuffer"); 
  if (!__glVertexArrayElementBuffer)
  {
    message_box(S("OpenGL"), S("Unable to load opengl function: glVertexArrayElementBuffer"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glProgramUniformMatrix3fv = (PFNglProgramUniformMatrix3fvPROC)_load_gl_function("glProgramUniformMatrix3fv"); 
  if (!__glProgramUniformMatrix3fv)
  {
    message_box(S("OpenGL"), S("Unable to load opengl function: glProgramUniformMatrix3fv"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glViewport = (PFNglViewportPROC)_load_gl_function("glViewport"); 
  if (!__glViewport)
  {
    message_box(S("OpenGL"), S("Unable to load opengl function: glViewport"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glBlendFunc = (PFNglBlendFuncPROC)_load_gl_function("glBlendFunc"); 
  if (!__glBlendFunc)
  {
    message_box(S("OpenGL"), S("Unable to load opengl function: glBlendFunc"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glClear = (PFNglClearPROC)_load_gl_function("glClear"); 
  if (!__glClear)
  {
    message_box(S("OpenGL"), S("Unable to load opengl function: glClear"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glValidateProgramPipeline = (PFNglValidateProgramPipelinePROC)_load_gl_function("glValidateProgramPipeline"); 
  if (!__glValidateProgramPipeline)
  {
    message_box(S("OpenGL"), S("Unable to load opengl function: glValidateProgramPipeline"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glGetProgramPipelineiv = (PFNglGetProgramPipelineivPROC)_load_gl_function("glGetProgramPipelineiv"); 
  if (!__glGetProgramPipelineiv)
  {
    message_box(S("OpenGL"), S("Unable to load opengl function: glGetProgramPipelineiv"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glGetProgramPipelineInfoLog = (PFNglGetProgramPipelineInfoLogPROC)_load_gl_function("glGetProgramPipelineInfoLog"); 
  if (!__glGetProgramPipelineInfoLog)
  {
    message_box(S("OpenGL"), S("Unable to load opengl function: glGetProgramPipelineInfoLog"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glDepthMask = (PFNglDepthMaskPROC)_load_gl_function("glDepthMask"); 
  if (!__glDepthMask)
  {
    message_box(S("OpenGL"), S("Unable to load opengl function: glDepthMask"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glUniformMatrix4fv = (PFNglUniformMatrix4fvPROC)_load_gl_function("glUniformMatrix4fv"); 
  if (!__glUniformMatrix4fv)
  {
    message_box(S("OpenGL"), S("Unable to load opengl function: glUniformMatrix4fv"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glUniform1i = (PFNglUniform1iPROC)_load_gl_function("glUniform1i"); 
  if (!__glUniform1i)
  {
    message_box(S("OpenGL"), S("Unable to load opengl function: glUniform1i"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glDepthFunc = (PFNglDepthFuncPROC)_load_gl_function("glDepthFunc"); 
  if (!__glDepthFunc)
  {
    message_box(S("OpenGL"), S("Unable to load opengl function: glDepthFunc"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glGenTextures = (PFNglGenTexturesPROC)_load_gl_function("glGenTextures"); 
  if (!__glGenTextures)
  {
    message_box(S("OpenGL"), S("Unable to load opengl function: glGenTextures"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glBindTexture = (PFNglBindTexturePROC)_load_gl_function("glBindTexture"); 
  if (!__glBindTexture)
  {
    message_box(S("OpenGL"), S("Unable to load opengl function: glBindTexture"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glTexImage2D = (PFNglTexImage2DPROC)_load_gl_function("glTexImage2D"); 
  if (!__glTexImage2D)
  {
    message_box(S("OpenGL"), S("Unable to load opengl function: glTexImage2D"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glTexParameteri = (PFNglTexParameteriPROC)_load_gl_function("glTexParameteri"); 
  if (!__glTexParameteri)
  {
    message_box(S("OpenGL"), S("Unable to load opengl function: glTexParameteri"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glPixelStorei = (PFNglPixelStoreiPROC)_load_gl_function("glPixelStorei"); 
  if (!__glPixelStorei)
  {
    message_box(S("OpenGL"), S("Unable to load opengl function: glPixelStorei"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glGenerateMipmap = (PFNglGenerateMipmapPROC)_load_gl_function("glGenerateMipmap"); 
  if (!__glGenerateMipmap)
  {
    message_box(S("OpenGL"), S("Unable to load opengl function: glGenerateMipmap"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glDisable = (PFNglDisablePROC)_load_gl_function("glDisable"); 
  if (!__glDisable)
  {
    message_box(S("OpenGL"), S("Unable to load opengl function: glDisable"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glGenFramebuffers = (PFNglGenFramebuffersPROC)_load_gl_function("glGenFramebuffers"); 
  if (!__glGenFramebuffers)
  {
    message_box(S("OpenGL"), S("Unable to load opengl function: glGenFramebuffers"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glFramebufferTexture2D = (PFNglFramebufferTexture2DPROC)_load_gl_function("glFramebufferTexture2D"); 
  if (!__glFramebufferTexture2D)
  {
    message_box(S("OpenGL"), S("Unable to load opengl function: glFramebufferTexture2D"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glGetTexLevelParameteriv = (PFNglGetTexLevelParameterivPROC)_load_gl_function("glGetTexLevelParameteriv"); 
  if (!__glGetTexLevelParameteriv)
  {
    message_box(S("OpenGL"), S("Unable to load opengl function: glGetTexLevelParameteriv"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glReadBuffer = (PFNglReadBufferPROC)_load_gl_function("glReadBuffer"); 
  if (!__glReadBuffer)
  {
    message_box(S("OpenGL"), S("Unable to load opengl function: glReadBuffer"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glReadPixels = (PFNglReadPixelsPROC)_load_gl_function("glReadPixels"); 
  if (!__glReadPixels)
  {
    message_box(S("OpenGL"), S("Unable to load opengl function: glReadPixels"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glGenQueries = (PFNglGenQueriesPROC)_load_gl_function("glGenQueries"); 
  if (!__glGenQueries)
  {
    message_box(S("OpenGL"), S("Unable to load opengl function: glGenQueries"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glGetQueryObjectui64v = (PFNglGetQueryObjectui64vPROC)_load_gl_function("glGetQueryObjectui64v"); 
  if (!__glGetQueryObjectui64v)
  {
    message_box(S("OpenGL"), S("Unable to load opengl function: glGetQueryObjectui64v"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glBeginQuery = (PFNglBeginQueryPROC)_load_gl_function("glBeginQuery"); 
  if (!__glBeginQuery)
  {
    message_box(S("OpenGL"), S("Unable to load opengl function: glBeginQuery"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glEndQuery = (PFNglEndQueryPROC)_load_gl_function("glEndQuery"); 
  if (!__glEndQuery)
  {
    message_box(S("OpenGL"), S("Unable to load opengl function: glEndQuery"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glBlendFuncSeparate = (PFNglBlendFuncSeparatePROC)_load_gl_function("glBlendFuncSeparate"); 
  if (!__glBlendFuncSeparate)
  {
    message_box(S("OpenGL"), S("Unable to load opengl function: glBlendFuncSeparate"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glFinish = (PFNglFinishPROC)_load_gl_function("glFinish"); 
  if (!__glFinish)
  {
    message_box(S("OpenGL"), S("Unable to load opengl function: glFinish"), S("OpenGL.cgen"), 0);
    return false;
  }

  __glObjectLabel = (PFNglObjectLabelPROC)_load_gl_function("glObjectLabel"); 
  if (!__glObjectLabel)
  {
    message_box(S("OpenGL"), S("Unable to load opengl function: glObjectLabel"), S("OpenGL.cgen"), 0);
    return false;
  }

  return true;
}