cmake_minimum_required(VERSION 3.10)

set(CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR})
set(CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(ArtiApp)

find_package(fmt CONFIG REQUIRED)
find_package(SFML CONFIG REQUIRED)

add_library(
    imgui-sfml STATIC
        deps/imgui/src/imgui.cpp
        deps/imgui/src/imgui_demo.cpp
        deps/imgui/src/imgui_draw.cpp
        deps/imgui/src/imgui_tables.cpp
        deps/imgui/src/imgui_stdlib.cpp
        deps/imgui/src/imgui_widgets.cpp
        deps/imgui/src/imgui-SFML.cpp
)

target_include_directories(
    imgui-sfml PUBLIC
        deps/imgui/include
)

if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set(GL_LIB GL)
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    set(GL_LIB openGL32)
else()
    message(FATAL_ERROR "Other system than Linux or Windows is not supported")
endif()

target_link_libraries(
    imgui-sfml PUBLIC
        SFML::SFML
        SFML::system
        SFML::window
        SFML::graphics
        ${GL_LIB}
)

add_library(
    ArtiApp STATIC
        include/app.hpp
        include/pixel.hpp
        include/imgui.hpp
        include/input.hpp
        include/renderer.hpp
        include/math/vec2d.hpp
        include/utils/utils.hpp
        include/utils/random.hpp
        include/utils/logger.hpp
        include/constants/keys.hpp
        include/constants/math.hpp
        include/constants/colors.hpp

        src/app.cpp
        src/input.cpp
        src/pixel.cpp
        src/renderer.cpp
)

target_compile_definitions(
    ArtiApp PUBLIC
        NDEBUG
        ARTI_MEASURE_FPS
)

target_include_directories(
    ArtiApp PUBLIC
        include
        deps/IconFonts
)

target_link_libraries(
    ArtiApp PUBLIC
        imgui-sfml
        fmt::fmt
)
