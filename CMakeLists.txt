cmake_minimum_required(VERSION 3.23)

project(ArtiApp)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(deps)
add_subdirectory(deps/fmt)

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
        fmt
)