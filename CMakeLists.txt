cmake_minimum_required(VERSION 3.31)
project(D3D11Engine)
set(CMAKE_CXX_STANDARD 20)

macro(gather_sources)
    file(GLOB INC src/*.h src/*.hpp)
    file(GLOB SRC CONFIGURE_DEPENDS src/*.cpp src/*.c)
endmacro()

gather_sources()
add_subdirectory(libs)

add_executable(${PROJECT_NAME}
        ${SRC}
        ${INC}
)
set_target_properties(${PROJECT_NAME} PROPERTIES WIN32_EXECUTABLE TRUE)

target_link_libraries(${PROJECT_NAME} PRIVATE glfw)

target_precompile_headers(${PROJECT_NAME} PRIVATE src/pch.hpp)