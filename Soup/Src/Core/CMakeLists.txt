set(sources)
set(headers)

# CLI
set(cli_dir ${CMAKE_CURRENT_SOURCE_DIR}/CLI)
list(APPEND public_includes ${cli_dir})

list(APPEND sources
    ${cli_dir}/CLIArguments.cpp
    ${cli_dir}/CLICommands.cpp
    ${cli_dir}/CLIConsole.cpp
    ${cli_dir}/CLICore.cpp)

list(APPEND headers
    ${cli_dir}/CLIArguments.h
    ${cli_dir}/CLICommands.h
    ${cli_dir}/CLIConsole.h
    ${cli_dir}/CLICore.h)

# Core
set(core_dir ${CMAKE_CURRENT_SOURCE_DIR}/Core)
list(APPEND sources

    # Event
    ${core_dir}/Event/ApplicationEvent.h
    ${core_dir}/Event/CLIEvent.h
    ${core_dir}/Event/Event.h
    ${core_dir}/Event/KeyEvent.h
    ${core_dir}/Event/MouseEvent.h

    # Log
    ${core_dir}/Log/ApplicationLogger.cpp
    ${core_dir}/Log/LogCore.cpp
    ${core_dir}/Log/Logger.cpp

    # Layer
    ${core_dir}/Layer/LayerStack.cpp

    # Memory
    ${core_dir}/Memory/Buffer.cpp
    ${core_dir}/Memory/BufferStreamWriter.cpp

    # Root
    ${core_dir}/Application.cpp
    ${core_dir}/Input.cpp
    ${core_dir}/Time.cpp
    ${core_dir}/Window.cpp)

list(APPEND headers

    # Layer
    ${core_dir}/Layer/Layer.h
    ${core_dir}/Layer/LayerStack.h

    # Log
    ${core_dir}/Log/ApplicationLogger.h
    ${core_dir}/Log/LogCore.h
    ${core_dir}/Log/Logger.h

    # Memory
    ${core_dir}/Memory/Buffer.h
    ${core_dir}/Memory/BufferStreamWriter.h

    # Root
    ${core_dir}/Application.h
    ${core_dir}/Core.h
    ${core_dir}/EntryPoint.h
    ${core_dir}/KeyCodes.h
    ${core_dir}/MouseButtons.h
    ${core_dir}/Input.h
    ${core_dir}/Time.h
    ${core_dir}/Window.h)

# Geoemtry
set(geometry_dir ${CMAKE_CURRENT_SOURCE_DIR}/Geometry)
list(APPEND sources
    ${geometry_dir}/Mesh.cpp)

list(APPEND headers
    ${geometry_dir}/Mesh.h)

# Gui
set(gui_dir ${CMAKE_CURRENT_SOURCE_DIR}/GUI)
list(APPEND sources
    ${gui_dir}/ImGuiLayer.cpp)

list(APPEND headers
    ${gui_dir}/ImGuiLayer.h)

# IO
set(io_dir ${CMAKE_CURRENT_SOURCE_DIR}/IO)
list(APPEND headers
    ${io_dir}/MeshImporter.h
    ${io_dir}/MeshReader.h
    ${io_dir}/ObjMeshReader.h)

list(APPEND sources
    ${io_dir}/MeshImporter.cpp
    ${io_dir}/ObjMeshReader.cpp)

# Metric
set(metric_dir ${CMAKE_CURRENT_SOURCE_DIR}/Metric)
list(APPEND headers
    ${metric_dir}/World.h)

# Renderer
set(renderer_dir ${CMAKE_CURRENT_SOURCE_DIR}/Renderer)
list(APPEND sources

    # Shader
    ${renderer_dir}/Shader/Shader.cpp
    ${renderer_dir}/Shader/ShaderLibrary.cpp

    # Root
    ${renderer_dir}/Camera.cpp
    ${renderer_dir}/FrameBuffer.cpp
    ${renderer_dir}/GpuMesh.cpp
    ${renderer_dir}/GraphicsContext.cpp
    ${renderer_dir}/IndexBuffer.cpp
    ${renderer_dir}/PerspectiveCamera.cpp
    ${renderer_dir}/Pipeline.cpp
    ${renderer_dir}/Renderer.cpp
    ${renderer_dir}/RenderPass.cpp
    ${renderer_dir}/ShaderStorageBuffer.cpp
    ${renderer_dir}/Texture2D.cpp
    ${renderer_dir}/TextureCubemap.cpp
    ${renderer_dir}/TextureUtils.cpp
    ${renderer_dir}/UniformBuffer.cpp
    ${renderer_dir}/VertexBuffer.cpp)

list(APPEND headers

    # Shader
    ${renderer_dir}/Shader/Shader.h
    ${renderer_dir}/Shader/ShaderLibrary.h

    # Root
    ${renderer_dir}/Camera.h
    ${renderer_dir}/FrameBuffer.h
    ${renderer_dir}/GpuMesh.h
    ${renderer_dir}/GraphicsContext.h
    ${renderer_dir}/IndexBuffer.h
    ${renderer_dir}/Lights.h
    ${renderer_dir}/PerspectiveCamera.h
    ${renderer_dir}/Pipeline.h
    ${renderer_dir}/Primitives.h
    ${renderer_dir}/Renderer.h
    ${renderer_dir}/RendererAPI.h
    ${renderer_dir}/RendererMaterial.h
    ${renderer_dir}/RenderPass.h
    ${renderer_dir}/ShaderStorageBuffer.h
    ${renderer_dir}/Texture.h
    ${renderer_dir}/Texture2D.h
    ${renderer_dir}/TextureCubemap.h
    ${renderer_dir}/TextureSpecifications.h
    ${renderer_dir}/TextureUtils.h
    ${renderer_dir}/UniformBuffer.h
    ${renderer_dir}/VertexBuffer.h)

# Scene
set(scene_dir ${CMAKE_CURRENT_SOURCE_DIR}/Scene)
list(APPEND sources
    ${scene_dir}/Entity.cpp
    ${scene_dir}/Scene.cpp)

list(APPEND headers
    ${scene_dir}/Components.h
    ${scene_dir}/Entity.h
    ${scene_dir}/Scene.h)

# Utils
set(utils_dir ${CMAKE_CURRENT_SOURCE_DIR}/Utils)
list(APPEND sources
    ${utils_dir}/FileUtils.cpp
    ${utils_dir}/MathUtils.cpp
    ${utils_dir}/StringUtils.cpp)

list(APPEND headers
    ${utils_dir}/FileUtils.h
    ${utils_dir}/MathUtils.h
    ${utils_dir}/StringUtils.h)

set(SP_CORE_SOURCES ${sources} PARENT_SCOPE)
set(SP_CORE_HEADERS ${headers} PARENT_SCOPE)
