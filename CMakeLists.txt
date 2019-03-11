cmake_minimum_required(VERSION 3.12.4)
project(COMP371_A3)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_BINARY_DIR build)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(GLFW3 REQUIRED)
find_package(GLM REQUIRED)

set(cpps "./src/main.cpp" "./src/Camera.cpp" "./src/Light.cpp")

add_executable(COMP371_A3 ${cpps} ${headers})

FILE(COPY shaders DESTINATION "${CMAKE_BINARY_DIR}")
FILE(COPY objects DESTINATION "${CMAKE_BINARY_DIR}")
FILE(COPY textures DESTINATION "${CMAKE_BINARY_DIR}")

target_link_libraries(${PROJECT_NAME} glfw glm glew ${OPENGL_LIBRARIES})

