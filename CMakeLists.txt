cmake_minimum_required(VERSION 3.0)
project(inteplas)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)

# Find SFML
find_package(SFML 2.5 COMPONENTS system window graphics REQUIRED)
if(SFML_FOUND)
    include_directories(${SFML_INCLUDE_DIR})
endif()

# Find TGUI
find_package(TGUI REQUIRED)
if(TGUI_FOUND)
    include_directories(${TGUI_INCLUDE_DIR})
endif()

# Find jsoncpp
find_package(PkgConfig REQUIRED)
pkg_search_module(JSONCPP REQUIRED jsoncpp)
if(JSONCPP_FOUND)
    include_directories(${JSONCPP_INCLUDE_DIRS})
endif()

# Find all C++ source files in the src directory
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Add executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Link SFML libraries
target_link_libraries(${PROJECT_NAME} sfml-system sfml-window sfml-graphics sfml-audio)

# Link TGUI libraries
target_link_libraries(${PROJECT_NAME} tgui)

# Link jsoncpp libraries
target_link_libraries(${PROJECT_NAME} ${JSONCPP_LIBRARIES})
