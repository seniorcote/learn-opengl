cmake_minimum_required(VERSION 3.0)
project(learn-opengl)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "-lm -Wall")

add_executable(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/src/main.c)

find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED COMPONENTS OpenGL)
find_package(GLEW REQUIRED)

target_link_libraries(${PROJECT_NAME} glfw ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES})