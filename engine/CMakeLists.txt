cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(Engine)

set(PROJECT_SOURCE_FILES
	source/engine.h
	source/engine.cpp
	source/application.h
	source/application.cpp
	source/common.h
	source/eng.h
	source/input/inputHandler.h
	source/input/inputHandler.cpp
	source/graphics/shaderProgram.h
	source/graphics/shaderProgram.cpp
	source/graphics/graphicsAPI.h
	source/graphics/graphicsAPI.cpp
	source/graphics/texture.h
	source/graphics/texture.cpp
	source/graphics/textureManager.h
	source/graphics/textureManager.cpp
	source/render/camera2D.h
	source/render/camera2D.cpp
	source/render/renderer2D.h
	source/render/renderer2D.cpp
	source/particles/ParticleType.h
	source/particles/Particle.h
	source/particles/Particle.cpp
	source/particles/Force.h
	source/particles/Force.cpp
	source/particles/Emitter.h
	source/particles/Emitter.cpp
	source/particles/ParticleSystem.h
	source/particles/ParticleSystem.cpp
)

foreach(FILE ${PROJECT_SOURCE_FILES}) 
    get_filename_component(PARENT_DIR "${FILE}" DIRECTORY)
    string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}" "" GROUP "${PARENT_DIR}")
    string(REPLACE "/" "\\" GROUP "${GROUP}")
    source_group("${GROUP}" FILES "${FILE}")
endforeach()

include_directories(source)
add_library(${PROJECT_NAME} ${PROJECT_SOURCE_FILES})

target_compile_definitions(${PROJECT_NAME} PRIVATE ENGINE_REPO_ROOT="${CMAKE_CURRENT_SOURCE_DIR}/..")

set(GLFW_BUILD_DOCS OFF CACHE BOOL "Disable GLFW docs" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "Disable GLFW examples" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "Disable GLFW tests" FORCE)

add_subdirectory(thirdparty/glfw "${CMAKE_CURRENT_BINARY_DIR}/glfw_build")
include_directories(thirdparty/glfw/include)

add_subdirectory(thirdparty/glad "${CMAKE_CURRENT_BINARY_DIR}/glad_build")
include_directories(thirdparty/glad/include)

add_subdirectory(thirdparty/glm-1.0.3)
include_directories(thirdparty/glm-1.0.3)

include(FetchContent)

FetchContent_Declare(
  imgui
  GIT_REPOSITORY https://github.com/ocornut/imgui.git
  GIT_TAG docking
)
FetchContent_GetProperties(imgui)
if(NOT imgui_POPULATED)
  FetchContent_Populate(imgui)
endif()

add_library(ImGui STATIC
  ${imgui_SOURCE_DIR}/imgui.cpp
  ${imgui_SOURCE_DIR}/imgui_draw.cpp
  ${imgui_SOURCE_DIR}/imgui_tables.cpp
  ${imgui_SOURCE_DIR}/imgui_widgets.cpp
  ${imgui_SOURCE_DIR}/imgui_demo.cpp
  ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
  ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)
target_include_directories(ImGui PUBLIC ${imgui_SOURCE_DIR} ${imgui_SOURCE_DIR}/backends)
target_link_libraries(ImGui PUBLIC glfw glad)

target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_BINARY_DIR})

target_link_libraries(${PROJECT_NAME} 
	glfw 
	glad
	glm
	ImGui
)