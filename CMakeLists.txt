cmake_minimum_required(VERSION 3.3)

project(OpenGL_concepts)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(CMAKE_MODULE_PATH /usr/local/lib/cmake /usr/local/lib/x86_64-linux-gnu/cmake)
set(CMAKE_PREFIX_PATH /usr/local/lib/cmake/glfw)

set(${GLM_DIR} /usr/local/lib/x86_64-linux-gnu/cmake/glm)
find_package (PkgConfig REQUIRED)
find_package (OpenGL REQUIRED)
find_package (glfw3 REQUIRED)
find_package (GLM REQUIRED)
find_package (GLEW REQUIRED STATIC)

set(SOURCE_FILES main.cpp)
#set(SOURCE_FILES main.cpp __add_other_cpp_files_here__)

add_executable(OpenGL_concepts ${SOURCE_FILES})
include_directories(${CMAKE_SOURCE_DIR})
target_link_libraries (OpenGL_concepts ${OPENGL_LIBRARIES} ${GLFW3_LIBRARY}
        ${GLEW_LIBRARY} Xxf86vm pthread Xrandr Xinerama Xi Xcursor)