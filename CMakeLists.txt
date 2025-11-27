cmake_minimum_required(VERSION 3.10)
project(illumination)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glm CONFIG REQUIRED)

set(SOURCES 
    src/main.cpp
    src/ObjectByCurveRotate.cpp
    src/VAO.cpp
    src/VBO.cpp
    src/EBO.cpp
    src/Shader.cpp
    src/Camera.cpp)

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(${PROJECT_NAME} PRIVATE
  glfw
  glad::glad
  imgui::imgui
  OpenGL::GL
  glm::glm
)
