project(inteplas)
set(CMAKE_CXX_STANDARD 11)

find_package(SFML 2.5 COMPONENTS system window graphics REQUIRED)
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

set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} sfml-system sfml-window sfml-graphics sfml-audio)
target_link_libraries(${PROJECT_NAME} tgui)
target_link_libraries(${PROJECT_NAME} ${JSONCPP_LIBRARIES})