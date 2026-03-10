cmake_minimum_required(VERSION 3.14)

project(hexagon-field VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIR ${CMAKE_BINARY_DIR}/$<CONFIGURATION:${PROJECT_NAME}-run>)
set(ASSETS_DIR ${CMAKE_SOURCE_DIR}/assets)
set(ASSETS_OUTPUT_DIR $<TARGET_FILE_DIR:${PROJECT_NAME}-run>/assets)
set(BINARY_OUTPUT_DIR $<TARGET_FILE_DIR:${PROJECT_NAME}-run>/bin)
set(CMAKE_DIR ${CMAKE_SOURCE_DIR}/cmake)
set(VENDORED_SOURCE_DIRECTORY ${CMAKE_SOURCE_DIR}/vendored)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_DIR})
set(
        SRC
        src/hexmap.h
        src/hexmap.cpp
        src/exception/sdl_exception.h
        src/sdl_engine.cpp
        src/sdl_engine.h
        src/ecs/system/base_system.h
        src/ecs/system_manager.h
        src/ecs/game_world.h
        src/event/game_quit_event.h
        src/event/dispatcher_invoker.h
        src/timer/sdl_timer.h
        src/timer/sdl_timer.cpp
        src/ecs/system/graphics_initialization_system.h
        src/ecs/system_manager.cpp
        src/ecs/game_world.cpp
        src/event/game_start_event.h
        src/ecs/component.h
        src/ecs/system/graphics_destroy_system.h
        src/ecs/system/load_configuration_system.h
        src/event/game_world_destroy_event.h
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

include(AddSDL)

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
        ${ASSETS_DIR} ${ASSETS_OUTPUT_DIR}
)