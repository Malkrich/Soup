# Vendor targets
add_subdirectory(Vendors)

set(soup_sources)
set(soup_headers)

# Core sources
add_subdirectory(Src/Core)
list(APPEND soup_sources ${SP_CORE_SOURCES})
list(APPEND soup_sources ${SP_CORE_HEADERS})

# Platform sources
add_subdirectory(Src/Platforms)
list(APPEND soup_sources ${SP_PLATFORMS_SOURCES})
list(APPEND soup_headers ${SP_PLATFORMS_HEADERS})

add_library(Soup ${soup_sources} ${soup_headers})
target_precompile_headers(Soup PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Src/Pch.h)

set_target_properties(Soup PROPERTIES
    CXX_STANDARD 20)

target_include_directories(Soup PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/Src/Core)
target_include_directories(Soup PRIVATE
    ${SOUP_VENDORS_DIR}/glfw/include
    ${CMAKE_CURRENT_SOURCE_DIR}/Src)

target_link_libraries(Soup PUBLIC ${SP_VENDORS_PUBLIC_TARGETS})
target_link_libraries(Soup PRIVATE ${SP_VENDORS_PRIVATE_TARGETS})

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(Soup PUBLIC
        SP_DEBUG)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_definitions(Soup PUBLIC
        SP_RELEASE)
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    target_compile_definitions(Soup PUBLIC
        SP_WINDOWS)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    target_compile_definitions(Soup PUBLIC
        SP_LINUX)
endif()
