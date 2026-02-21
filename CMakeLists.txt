cmake_minimum_required(VERSION 3.28)
project(hexagon-field LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.2
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

set(SRC
        src/main.cpp
        src/hexmap.h
        src/hexmap.cpp
)
add_executable(${PROJECT_NAME}-run ${SRC})
target_compile_features(${PROJECT_NAME}-run PRIVATE cxx_std_17)
target_link_libraries(${PROJECT_NAME}-run PRIVATE SFML::Graphics)

add_custom_command(TARGET ${PROJECT_NAME}-run PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/resources/ $<TARGET_FILE_DIR:${PROJECT_NAME}-run>/resources/)