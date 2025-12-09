cmake_minimum_required(VERSION 3.14)
project(MatchSolution)

# C/C++ Standards
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(MATCH_GAME match_game)
set(SECOND_EXAMPLE second_example)


# Platform-specific flags
if(APPLE)
    add_compile_options(-Wno-deprecated-declarations -O3 -Wall -Wextra)
elseif(UNIX AND NOT APPLE)
    add_compile_options(-O3 -Wall -Wextra -pthread)
endif()

# SDL3 - Try system, else fetch from source
find_package(SDL3 QUIET)
if(NOT SDL3_FOUND)
    message(STATUS "SDL3 not found, downloading...")
    include(FetchContent)
    FetchContent_Declare(
        SDL3
        GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
        GIT_TAG release-3.2.0
        GIT_SHALLOW TRUE
    )
    FetchContent_MakeAvailable(SDL3)
    add_compile_definitions(SDL3_BUILT_FROM_SOURCE)
else()
    message(STATUS "Found system SDL3")
endif()

# Spinach
# Collect C++ source files for spinach
file(GLOB_RECURSE SPINACH_CPP_SRC CONFIGURE_DEPENDS
    spinach/common/*.cpp
    spinach/core/*.cpp
)


# Collect C sources for spinach
file(GLOB_RECURSE SPINACH_C_SRC CONFIGURE_DEPENDS
    spinach/external/stb/*.c
)

# Collect headers 
file(GLOB_RECURSE SPINACH_H CONFIGURE_DEPENDS
    spinach/common/*.[h|hpp]
    spinach/core/*.[h|hpp]
    spinach/external/stb/*.h
	spinach/external/nanosvg/*.h
	spinach/external/miniaudio/*.h
)


# 1. match_game
# Collect C++ source files
file(GLOB_RECURSE MATCH_GAME_CPP_SRC CONFIGURE_DEPENDS
    match_game/*.cpp
)

# Collect headers 
file(GLOB_RECURSE MATCH_GAME_CPP_H CONFIGURE_DEPENDS
	match_game/*.[h|hpp]
)


# Create the executable
add_executable(	${MATCH_GAME}
	${MATCH_GAME_CPP_SRC}
	${SPINACH_CPP_SRC}
    ${SPINACH_C_SRC}
)


target_sources(${MATCH_GAME} PRIVATE
    ${MATCH_GAME_CPP_H}
    ${SPINACH_H}
)

target_include_directories(${MATCH_GAME} PRIVATE
    spinach/common
    spinach/core
    spinach/external/stb
	spinach/external/nanosvg
	spinach/external/miniaudio
	match_game
)


target_link_libraries(${MATCH_GAME} PRIVATE SDL3::SDL3)

