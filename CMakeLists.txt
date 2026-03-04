cmake_minimum_required(VERSION 3.7)
project(hexagon-field VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/$<CONFIGURATION>)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/$<CONFIGURATION>)
set(RESOURCES_DIRECTORY ${CMAKE_SOURCE_DIR}/resources)
set(RESOURCES_OUTPUT_DIRECTORY $<TARGET_FILE_DIR:${PROJECT_NAME}-run>/resources)
set(CMAKE_FILES_DIRECTORY ${CMAKE_SOURCE_DIR}/cmake)
set(VENDORED_FILES_DIRECTORY ${CMAKE_SOURCE_DIR}/vendored)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_FILES_DIRECTORY})
set(
        SRC
        src/main.cpp
        src/hexmap.h
        src/hexmap.cpp
        src/exception/sdl_exception.h
        src/sdl_engine.cpp
        src/sdl_engine.h
        src/ecs/systems/base_system.h
        src/ecs/system_manager.h
        src/ecs/world.h
        src/event/game_quit_event.h
        src/event/dispatcher_invoker.h
        src/timer/sdl_timer.h
        src/timer/sdl_timer.cpp
        src/ecs/systems/graphics_initialization_system.h
        src/ecs/system_manager.cpp
        src/ecs/world.cpp
        src/event/game_start_event.h
        src/ecs/components.h
        src/ecs/systems/graphics_destroy_system.h
        src/ecs/system_manager.ipp
        src/ecs/systems/load_configuration_system.h
)

add_executable(${PROJECT_NAME}-run ${SRC})

target_compile_features(${PROJECT_NAME}-run PRIVATE cxx_std_23)

set_target_properties(
        ${PROJECT_NAME}-run PROPERTIES
        CXX_STANDARD 23
        CMAKE_CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
)

include(AddSDL)

include(FetchContent)

FetchContent_Declare(
        EnTT
        GIT_REPOSITORY https://github.com/skypjack/entt.git
        GIT_TAG v3.16.0
        EXCLUDE_FROM_ALL
        SYSTEM
)
FetchContent_MakeAvailable(EnTT)
target_link_libraries(${PROJECT_NAME}-run PRIVATE EnTT::EnTT)

FetchContent_Declare(
        GLM
        GIT_REPOSITORY https://github.com/g-truc/glm.git
        GIT_TAG 1.0.3
        EXCLUDE_FROM_ALL
        SYSTEM
)
FetchContent_MakeAvailable(GLM)
target_link_libraries(${PROJECT_NAME}-run PRIVATE glm::glm)

FetchContent_Declare(
        LIBFYAML
        GIT_REPOSITORY https://github.com/pantoniou/libfyaml.git
        GIT_TAG v0.9.5
        EXCLUDE_FROM_ALL
        SYSTEM
)
FetchContent_MakeAvailable(LIBFYAML)
target_link_libraries(${PROJECT_NAME}-run PRIVATE libfyaml::libfyaml)

add_custom_command(
        TARGET ${PROJECT_NAME}-run PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${RESOURCES_DIRECTORY} ${RESOURCES_OUTPUT_DIRECTORY}
)