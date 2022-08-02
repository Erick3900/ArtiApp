cmake_minimum_required(VERSION 3.20)

project(ArtiApp-deps)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(sfml)

add_library(
    imgui-sfml STATIC
        "imgui/src/imgui.cpp"
        "imgui/src/imgui_demo.cpp"
        "imgui/src/imgui_draw.cpp"
        "imgui/src/imgui_tables.cpp"
        "imgui/src/imgui_stdlib.cpp"
        "imgui/src/imgui_widgets.cpp"
        "imgui/src/imgui-SFML.cpp"
)

target_include_directories(
    imgui-sfml PUBLIC
        "imgui/include/"
        "sfml/include/"
)

if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set(GL_LIB GL)
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    set(GL_LIB openGL32)
endif()

target_link_libraries(
    imgui-sfml PUBLIC
        sfml-system
        sfml-window
        sfml-graphics
        ${GL_LIB}
)