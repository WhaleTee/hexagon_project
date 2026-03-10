#set(SDLTTF_VENDORED ON)
#set(SDLSHADERCROSS_VENDORED ON)
#set(SDLSHADERCROSS_SPIRVCROSS_SHARED OFF)

#add_subdirectory(${VENDORED_FILES_DIRECTORY}/SDL EXCLUDE_FROM_ALL)
#add_subdirectory(${VENDORED_FILES_DIRECTORY}/SDL_ttf EXCLUDE_FROM_ALL)
#add_subdirectory(${VENDORED_FILES_DIRECTORY}/SDL_shadercross/external/DirectXShaderCompiler EXCLUDE_FROM_ALL)
#add_subdirectory(${VENDORED_FILES_DIRECTORY}/SDL_shadercross/external/SPIRV-Headers EXCLUDE_FROM_ALL)
#add_subdirectory(${VENDORED_FILES_DIRECTORY}/SDL_shadercross/external/SPIRV-Tools EXCLUDE_FROM_ALL)
#add_subdirectory(${VENDORED_FILES_DIRECTORY}/SDL_shadercross/external/SPIRV-Cross EXCLUDE_FROM_ALL)
#add_subdirectory(${VENDORED_FILES_DIRECTORY}/SDL_shadercross EXCLUDE_FROM_ALL)

#target_link_directories(
#        ${PROJECT_NAME}-run PRIVATE
#        ${VENDORED_FILES_DIRECTORY}/SDL
#        ${VENDORED_FILES_DIRECTORY}/SDL_ttf
#        ${VENDORED_FILES_DIRECTORY}/SDL_shadercross
#)
find_package(Python3 REQUIRED)
# properties --------
# ---- SDL
set(SDL_GIT_TAG release-3.4.2)

#---- SDL_shadercross
set(SDL_shadercross_GIT_URL https://github.com/libsdl-org/SDL_shadercross)
set(SDL_shadercross_GIT_TAG origin/main)
set(SDL_shadercross_SOURCE_DIR ${VENDORED_SOURCE_DIRECTORY}/SDL_shadercross)
set(SDL_shadercross_BINARY_DIR ${VENDORED_SOURCE_DIRECTORY}/SDL_shadercross/build)

# fetch, configure and build libraries --------
# ---- SDL

FetchContent_Declare(
        SDL3
        GIT_REPOSITORY https://github.com/libsdl-org/SDL
        GIT_TAG ${SDL_GIT_TAG}
        GIT_SHALLOW TRUE

        EXCLUDE_FROM_ALL
        SYSTEM
)

FetchContent_MakeAvailable(SDL3)

target_link_libraries(${PROJECT_NAME}-run PRIVATE SDL3:SDL3)

#set(DirectXShaderCompiler_llvm-build_SOURCE_DIR ${CMAKE_CURRENT_BINARY_DIR}/_deps/sdl_shadercross-src/external/DirectXShaderCompiler/utils/llvm-build)

# ---- SDL_shadercross
#FetchContent_Declare(
#        SDL_shadercross
#        GIT_REPOSITORY https://github.com/libsdl-org/SDL_shadercross.git
#        GIT_TAG ${SDL_shadercross_GIT_TAG}
#        GIT_SHALLOW TRUE
#        GIT_SUBMODULES_RECURSIVE TRUE
#        PATCH_COMMAND ${Python3_EXECUTABLE} ${VENDORED_FILES_DIRECTORY}/SDL_shadercross/fix_llvm-build.py ${DirectXShaderCompiler_llvm-build_SOURCE_DIR}/llvm-build
#        EXCLUDE_FROM_ALL
#        SYSTEM
#)

set(SDLSHADERCROSS_DXC ON)
set(SDLSHADERCROSS_VENDORED ON)
set(SDLSHADERCROSS_SPIRVCROSS_SHARED OFF)
set(DXC_USE_LIT ON)
set(SDL3_DIR ${sdl3_BINARY_DIR})
set(CMAKE_OBJECT_PATH_MAX 350)

#FetchContent_MakeAvailable(SDL_shadercross)

if (NOT SDL_shadercross_POPULATED)
    if (NOT SDL_shadercross_SOURCE_DIR_EXISTS)
        execute_process(
                COMMAND git clone --recursive ${SDL_shadercross_GIT_URL} ${SDL_shadercross_SOURCE_DIR}
                WORKING_DIRECTORY ${VENDORED_SOURCE_DIRECTORY}
                RESULT_VARIABLE SDL_shadercross_GIT_CLONE_RESULT
                ERROR_VARIABLE SDL_shadercross_GIT_CLONE_ERROR
        )

        if (NOT SDL_shadercross_GIT_CLONE_RESULT EQUAL 0)
            message(FATAL_ERROR "git clone SDL_shadercross failed. ${SDL_shadercross_GIT_CLONE_RESULT}:\n${SDL_shadercross_GIT_CLONE_ERROR}\n")
        endif ()
        set(SDL_shadercross_SOURCE_DIR_EXISTS ON)
    endif ()

    execute_process(
            COMMAND git fetch origin
            WORKING_DIRECTORY ${SDL_shadercross_SOURCE_DIR}
            RESULT_VARIABLE SDL_shadercross_GIT_FETCH_RESULT
            ERROR_VARIABLE SDL_shadercross_GIT_FETCH_ERROR
    )

    if (NOT SDL_shadercross_GIT_FETCH_RESULT EQUAL 0)
        message(FATAL_ERROR "git fetch SDL_shadercross failed. ${SDL_shadercross_GIT_FETCH_RESULT}:\n${SDL_shadercross_GIT_FETCH_ERROR}\n")
    endif ()

    execute_process(
            COMMAND git reset --hard --recurse-submodules ${SDL_shadercross_GIT_TAG}
            WORKING_DIRECTORY ${SDL_shadercross_SOURCE_DIR}
            RESULT_VARIABLE SDL_shadercross_GIT_RESET_RESULT
            ERROR_VARIABLE SDL_shadercross_GIT_RESET_ERROR
    )

    if (NOT SDL_shadercross_GIT_RESET_RESULT EQUAL 0)
        message(FATAL_ERROR "git reset SDL_shadercross failed. ${SDL_shadercross_GIT_RESET_RESULT}:\n${SDL_shadercross_GIT_RESET_ERROR}\n")
    endif ()

    execute_process(
            COMMAND cmake -B ${SDL_shadercross_BINARY_DIR} -G Ninja -S .
                -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
                -DSDLSHADERCROSS_DXC=ON
                -DSDLSHADERCROSS_VENDORED=ON
                -DSDL3_DIR=${sdl3_BINARY_DIR}
            WORKING_DIRECTORY ${SDL_shadercross_SOURCE_DIR}
            RESULT_VARIABLE SDL_shadercross_CONFIGURE_RESULT
            ERROR_VARIABLE SDL_shadercross_CONFIGURE_ERROR
    )

    if (NOT SDL_shadercross_CONFIGURE_RESULT)
        message(FATAL_ERROR "configure SDL_shadercross failed. ${SDL_shadercross_CONFIGURE_RESULT}:\n${SDL_shadercross_CONFIGURE_ERROR}\n")
    endif ()

    add_custom_command(
            TARGET ${PROJECT_NAME}-run PRE_BUILD
            COMMAND cmake --build ${SDL_shadercross_BINARY_DIR}
            WORKING_DIRECTORY ${SDL_shadercross_SOURCE_DIR}
    )

    set(SDL_shadercross_POPULATED ON)
endif ()

target_link_libraries(${PROJECT_NAME}-run PRIVATE SDL3_shadercross:SDL3_shadercross)

# install SDL_shadercross --------
# ---- declare a helper function
function(install_file SOURCE_FILE DESTINATION_DIR)
    get_filename_component(FILENAME ${SOURCE_FILE} NAME)
    install(FILES ${SOURCE_FILE} DESTINATION ${DESTINATION_DIR} RENAME ${FILENAME})
endfunction()

# ---- install shadercross
set(SPIRV-Cross_SOURCE_DIR ${SDL_shadercross_BINARY_DIR}/external/SPIRV-Cross)
set(DirectXShaderCompiler_BINARY_DIR ${SDL_shadercross_BINARY_DIR}/external/DirectXShaderCompiler/bin)
install_file(${SPIRV-Cross_SOURCE_DIR}/spirv-cross-c-shared.dll ${BINARY_OUTPUT_DIR})
install_file(${SDL_shadercross_BINARY_DIR}/shadercross.exe ${BINARY_OUTPUT_DIR})
install_file(${DirectXShaderCompiler_BINARY_DIR}/dxcompiler.dll ${BINARY_OUTPUT_DIR})
install_file(${SDL3_BINARY_DIR}/SDL3.dll ${BINARY_OUTPUT_DIR})