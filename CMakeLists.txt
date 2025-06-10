cmake_minimum_required(VERSION 3.31)
project(D3D11Engine)
set(CMAKE_CXX_STANDARD 20)

add_subdirectory(libs)

add_executable(${PROJECT_NAME}
        src/main.cpp
        src/Application.cpp
        src/Application.h
        src/error.h
        src/System.h
        src/Window.cpp
        src/Window.h
        src/InputSystem.cpp
        src/InputSystem.h
)
set_target_properties(${PROJECT_NAME} PROPERTIES WIN32_EXECUTABLE TRUE)

target_link_libraries(${PROJECT_NAME} PRIVATE glfw)
