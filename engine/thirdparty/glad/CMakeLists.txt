cmake_minimum_required(VERSION 3.10)
project(glad)

# Add GLAD as a library
add_library(glad STATIC
	src/glad.c
)

# Include directories for GLAD
target_include_directories(glad PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Link with necessary libraries (OpenGL)
if(WIN32)
	target_link_libraries(glad PRIVATE opengl32)
elseif(APPLE)
	target_link_libraries(glad PRIVATE "-framework OpenGL")
else()
	target_link_libraries(glad PRIVATE GL)
endif()
