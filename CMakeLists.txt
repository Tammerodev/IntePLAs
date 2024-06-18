cmake_minimum_required(VERSION 3.16)

# Set C and C++ standards
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD 17)

# Declare project
project(inteplas LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Turn "build shared libs" off
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

include(FetchContent)

# Fetch SFML
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

# Fetch TGUI
FetchContent_Declare(
    TGUI
    GIT_REPOSITORY https://github.com/texus/TGUI.git
    GIT_TAG v0.9
)
FetchContent_MakeAvailable(TGUI)

# Fetch JsonCpp
FetchContent_Declare(
    JsonCpp
    GIT_REPOSITORY https://github.com/open-source-parsers/jsoncpp.git
    GIT_TAG 1.9.4
)
FetchContent_MakeAvailable(JsonCpp)

file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} PRIVATE
    sfml-system 
    sfml-window 
    sfml-graphics 
    sfml-audio
    tgui
    jsoncpp_lib
)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)
install(TARGETS ${PROJECT_NAME})

if(WIN32)
    add_custom_command(
        TARGET main
        COMMENT "Copy OpenAL DLL"
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:main>
        VERBATIM)
endif()