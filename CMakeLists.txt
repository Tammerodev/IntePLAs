cmake_minimum_required(VERSION 3.16)

# Set C and C++ standards
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD 17)

# Compiler flags -g=Debug, -03=Level 3 Optimizations
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

# Declare project
project(inteplas LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Turn "build shared libs" off
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

if(UNIX)
    # Include SFML
    if(SFML_FOUND)
        include_directories(${SFML_INCLUDE_DIR})
    endif()

    # Include TGUI
    find_package(TGUI REQUIRED)
    if(TGUI_FOUND)
        include_directories(${TGUI_INCLUDE_DIR})
    endif()

    # Include JsonCpp via PkgConfig
    find_package(PkgConfig REQUIRED)
    pkg_search_module(JSONCPP REQUIRED jsoncpp)
    if(JSONCPP_FOUND)
        include_directories(${JSONCPP_INCLUDE_DIRS})
    endif()
endif()

# Find all .cpp files in src folder
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Add executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE
    sfml-system 
    sfml-window 
    sfml-graphics 
    sfml-audio
    tgui
    jsoncpp
)

# Set c++ standard
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)

install(TARGETS ${PROJECT_NAME})