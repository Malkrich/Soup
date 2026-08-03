set(public_targets)
set(private_targets)

# entt
add_subdirectory(entt)
list(APPEND public_targets entt)

# glad
add_library(glad STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/glad/src/glad.c
    ${CMAKE_CURRENT_SOURCE_DIR}/glad/include/glad/glad.h
    ${CMAKE_CURRENT_SOURCE_DIR}/glad/include/KHR/khrplatform.h)

target_include_directories(glad PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/glad/include
    ${CMAKE_CURRENT_SOURCE_DIR}/glad/KHR)
list(APPEND private_targets glad)

# glfw
set(GLFW_BUILD_EXAMPLES OFF)
set(GLFW_BUILD_TESTS OFF)
add_subdirectory(glfw)
list(APPEND private_targets glfw)

# glm
set(GLM_ENABLE_CXX_20 ON)
add_subdirectory(glm)
list(APPEND public_targets glm)

# imgui
set(imgui_dir ${CMAKE_CURRENT_SOURCE_DIR}/imgui)
set(imgui_sources
    ${imgui_dir}/imgui_demo.cpp
    ${imgui_dir}/imgui_draw.cpp
    ${imgui_dir}/imgui_tables.cpp
    ${imgui_dir}/imgui_widgets.cpp
    ${imgui_dir}/imgui.cpp)

set(imgui_headers
    ${imgui_dir}/imconfig.h
    ${imgui_dir}/imgui_internal.h
    ${imgui_dir}/imgui.h
    ${imgui_dir}/imstb_rectpack.h
    ${imgui_dir}/imstb_textedit.h
    ${imgui_dir}/imstb_truetype.h)

set(imgui_backend_sources
    ${imgui_dir}/backends/imgui_impl_glfw.cpp
    ${imgui_dir}/backends/imgui_impl_opengl3.cpp)

set(imgui_backend_headers
    ${imgui_dir}/backends/imgui_impl_glfw.h
    ${imgui_dir}/backends/imgui_impl_opengl3.h)

add_library(imgui
    ${imgui_sources} ${imgui_headers}
    ${imgui_backend_sources} ${imgui_backend_headers})

target_include_directories(imgui PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/glfw/include)
target_include_directories(imgui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/imgui)
list(APPEND public_targets imgui)

# ImGuizmo
set(imguizmo_sources
    ${CMAKE_CURRENT_SOURCE_DIR}/ImGuizmo/ImGuizmo.cpp)

set(imguizmo_headers
    ${CMAKE_CURRENT_SOURCE_DIR}/ImGuizmo/ImGuizmo.h)

add_library(imguizmo ${imguizmo_sources} ${imguizmo_headers})
target_include_directories(imguizmo PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/ImGuizmo)
target_include_directories(imguizmo PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/imgui)
target_link_libraries(imguizmo PRIVATE imgui)
list(APPEND public_targets imguizmo)

# json
add_subdirectory(json)
list(APPEND private_targets nlohmann_json)

# spdlog
add_subdirectory(spdlog)
list(APPEND public_targets spdlog)

# stb_image
add_library(stb_image STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/stb_image/stb_image.h
    ${CMAKE_CURRENT_SOURCE_DIR}/stb_image/stb_image.cpp)
target_include_directories(stb_image PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/stb_image)
list(APPEND private_targets stb_image)

set(SP_VENDORS_PUBLIC_TARGETS ${public_targets} PARENT_SCOPE)
set(SP_VENDORS_PRIVATE_TARGETS ${private_targets} PARENT_SCOPE)
