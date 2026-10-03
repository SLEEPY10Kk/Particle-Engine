cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project (ParticleEngine)


set(SIMULATION_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/source" CACHE PATH
	"Folder containing this executable's game-specific source files")
set(SIMULATION_EXE_NAME "ParticleEngine" CACHE STRING
	"Name of the executable produced from SIMULATION_SOURCE_DIR")

file(GLOB_RECURSE SIMULATION_SOURCE_FILES
	"${SIMULATION_SOURCE_DIR}/*.h"
	"${SIMULATION_SOURCE_DIR}/*.cpp"
)

include_directories(engine/thirdparty/glm-1.0.3)
include_directories(${SIMULATION_SOURCE_DIR})
add_executable(${SIMULATION_EXE_NAME} ${SIMULATION_SOURCE_FILES})

target_compile_definitions(${SIMULATION_EXE_NAME} PRIVATE PROJECT_ROOT="${SIMULATION_SOURCE_DIR}/..")

add_subdirectory(engine "${CMAKE_CURRENT_BINARY_DIR}/engine_build")
include_directories(engine/source)

target_link_libraries(${SIMULATION_EXE_NAME} 
    Engine
)