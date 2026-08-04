set(sources)
set(headers)

# OpenGL
set(opengl_dir ${CMAKE_CURRENT_SOURCE_DIR}/OpenGL)

list(APPEND sources
    ${opengl_dir}/OpenGLFrameBuffer.h
    ${opengl_dir}/OpenGLContext.cpp
    ${opengl_dir}/OpenGLError.cpp
    ${opengl_dir}/OpenGLFrameBuffer.cpp
    ${opengl_dir}/OpenGLIndexBuffer.cpp
    ${opengl_dir}/OpenGLPipeline.cpp
    ${opengl_dir}/OpenGLRendererAPI.cpp
    ${opengl_dir}/OpenGLRenderPass.cpp
    ${opengl_dir}/OpenGLShader.cpp
    ${opengl_dir}/OpenGLShaderStorageBuffer.cpp
    ${opengl_dir}/OpenGLTexture2D.cpp
    ${opengl_dir}/OpenGLTextureCubemap.cpp
    ${opengl_dir}/OpenGLTextureUtils.cpp
    ${opengl_dir}/OpenGLUniformBuffer.cpp
    ${opengl_dir}/OpenGLVertexArray.cpp
    ${opengl_dir}/OpenGLVertexBuffer.cpp)

list(APPEND headers
    ${opengl_dir}/OpenGLContext.h
    ${opengl_dir}/OpenGLError.h
    ${opengl_dir}/OpenGLIndexBuffer.h
    ${opengl_dir}/OpenGLPipeline.h
    ${opengl_dir}/OpenGLRendererAPI.h
    ${opengl_dir}/OpenGLRenderPass.h
    ${opengl_dir}/OpenGLShader.h
    ${opengl_dir}/OpenGLShaderStorageBuffer.h
    ${opengl_dir}/OpenGLTexture2D.h
    ${opengl_dir}/OpenGLTextureCubemap.h
    ${opengl_dir}/OpenGLTextureUtils.h
    ${opengl_dir}/OpenGLUniformBuffer.h
    ${opengl_dir}/OpenGLVertexArray.h
    ${opengl_dir}/OpenGLVertexBuffer.h)

# ImGui
list (APPEND headers ${CMAKE_CURRENT_SOURCE_DIR}/ImGui/ImGui.h)

set(SP_PLATFORMS_SOURCES ${sources} PARENT_SCOPE)
set(SP_PLATFORMS_HEADERS ${headers} PARENT_SCOPE)
