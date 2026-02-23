cmake_minimum_required(VERSION 3.28)
project(hexagon-field VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(RESOURCES_DIRECTORY ${CMAKE_SOURCE_DIR}/resources/)
set(RESOURCES_OUTPUT_DIRECTORY $<TARGET_FILE_DIR:${PROJECT_NAME}-run>/resources/)
set(
        SRC
        src/main.cpp
        src/hexmap.h
        src/hexmap.cpp
        src/components.cpp
)

add_executable(${PROJECT_NAME}-run ${SRC})

target_compile_features(${PROJECT_NAME}-run PRIVATE cxx_std_23)

set_target_properties(
        ${PROJECT_NAME}-run PROPERTIES
        CXX_STANDARD 23
        CMAKE_CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
)

include(FetchContent)

FetchContent_Declare(
        SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 3.0.2
        GIT_SHALLOW ON
        EXCLUDE_FROM_ALL
        SYSTEM
)
FetchContent_MakeAvailable(SFML)
target_link_libraries(${PROJECT_NAME}-run PRIVATE SFML::Graphics)

FetchContent_Declare(
        GLM
        GIT_REPOSITORY https://github.com/g-truc/glm.git
        GIT_TAG 1.0.3
        EXCLUDE_FROM_ALL
        SYSTEM
)
FetchContent_MakeAvailable(GLM)
target_link_libraries(${PROJECT_NAME}-run PRIVATE glm::glm)

add_custom_command(
        TARGET ${PROJECT_NAME}-run PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${RESOURCES_DIRECTORY} ${RESOURCES_OUTPUT_DIRECTORY}
)