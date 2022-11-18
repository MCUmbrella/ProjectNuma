cmake_minimum_required(VERSION 3.18)
project(ProjectNuma)

set(CMAKE_CXX_STANDARD 11)

include(FindPkgConfig)
pkg_search_module(SDL2 REQUIRED sdl2)
pkg_search_module(SDL2_MIXER REQUIRED SDL2_mixer)
pkg_search_module(SDL2_IMAGE REQUIRED SDL2_image)
pkg_search_module(SDL2_TTF REQUIRED SDL2_ttf)

add_compile_options(-Wall -Werror=maybe-uninitialized -Warray-bounds)

Include_Directories(
        ${SDL2_INCLUDE_DIRS}
        ${SDL2_MIXER_INCLUDE_DIRS}
        ${SDL2_IMAGE_INCLUDE_DIRS}
        ${SDL2_TTF_INCLUDE_DIRS}
)

file(GLOB SOURCES   src/* src*/*)
Add_Executable (${CMAKE_PROJECT_NAME} ${SOURCES})

Target_Link_Libraries (
        ${CMAKE_PROJECT_NAME}
        ${SDL2_LIBRARIES}
        ${SDL2_MIXER_LIBRARIES}
        ${SDL2_IMAGE_LIBRARIES}
        ${SDL2_TTF_LIBRARIES}
        m
)
