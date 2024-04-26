cmake_minimum_required(VERSION 3.0)
project(inteplas)

set(CMAKE_CXX_STANDARD 11)

find_package(SFML 2.5 COMPONENTS system window graphics REQUIRED)
find_package(TGUI REQUIRED)
find_package(PkgConfig REQUIRED)

if(SFML_FOUND AND TGUI_FOUND AND JSONCPP_FOUND)
    include_directories(${SFML_INCLUDE_DIR} ${TGUI_INCLUDE_DIR} ${JSONCPP_INCLUDE_DIRS})

    set(CMAKE_CXX_FLAGS_DEBUG "-g")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3")

    file(GLOB_RECURSE SOURCES "src/*.cpp")

    add_executable(${PROJECT_NAME} ${SOURCES})

    target_link_libraries(${PROJECT_NAME} sfml-system sfml-window sfml-graphics sfml-audio)
    target_link_libraries(${PROJECT_NAME} tgui)
    target_link_libraries(${PROJECT_NAME} ${JSONCPP_LIBRARIES})
else()
    message(FATAL_ERROR "Required libraries not found")
endif()
