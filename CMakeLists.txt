cmake_minimum_required(VERSION 3.22)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD 17)

project(inteplas)

if(UNIX)
    if(SFML_FOUND)
        include_directories(${SFML_INCLUDE_DIR})
    endif()

    find_package(TGUI REQUIRED)
    if(TGUI_FOUND)
        include_directories(${TGUI_INCLUDE_DIR})
    endif()

    find_package(PkgConfig REQUIRED)
    pkg_search_module(JSONCPP REQUIRED jsoncpp)
    if(JSONCPP_FOUND)
        include_directories(${JSONCPP_INCLUDE_DIRS})
    endif()
endif()

set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(${PROJECT_NAME} ${SOURCES})

# Link libraries
target_link_libraries(${PROJECT_NAME} PUBLIC
    sfml-system 
    sfml-window 
    sfml-graphics 
    sfml-audio
    tgui
    jsoncpp
)