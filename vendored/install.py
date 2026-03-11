import os
import shutil
import subprocess
import sys

WORKING_DIR = os.getcwd()
BIN_INSTALL_DIR = os.path.join(WORKING_DIR, 'build')
LIB_INSTALL_DIR = BIN_INSTALL_DIR
CMAKE_EXE = os.path.realpath("C:/Users/nrobd/.jetbrains-toolbox/programs/CLion/bin/cmake/win/x64/bin/cmake.exe")
NINJA_EXE = os.path.realpath("C:/Users/nrobd/.jetbrains-toolbox/programs/CLion/bin/ninja/win/x64/ninja.exe")
CMD_FOR_VS = os.path.realpath("C:/Program Files/Microsoft Visual Studio/18/Community/Common7/Tools/VsDevCmd.bat")


def main():
    os.makedirs(WORKING_DIR, exist_ok=True)
    os.makedirs(BIN_INSTALL_DIR, exist_ok=True)

    # clone_and_build_sdl()

    clone_and_build_shadercross()

    install_binaries_windows()

    cleanup()


def clone_and_build_sdl():
    sdl_dir = os.path.join(WORKING_DIR, "SDL")
    if not os.path.exists(sdl_dir):
        subprocess.run(["git", "clone", "https://github.com/libsdl-org/SDL", sdl_dir])

    subprocess.run(["git", "fetch", "origin"], cwd=sdl_dir)
    subprocess.run(["git", "reset", "--hard", 'release-3.4.2'], cwd=sdl_dir)

    build_dir = os.path.join(sdl_dir, "build")
    if os.path.exists(build_dir):
        shutil.rmtree(build_dir)

    os.makedirs(build_dir)
    subprocess.run([CMD_FOR_VS, "&", "cmake", "-B build", "-S .",
                    # "-G Ninja",
                    # "-DCMAKE_MAKE_PROGRAM=" + NINJA_EXE,
                    "-DCMAKE_BUILD_TYPE=Release",
                    # "-DCMAKE_C_COMPILER=C:/Program Files/LLVM/bin/clang-cl.exe",
                    # "-DCMAKE_CXX_COMPILER=C:/Program Files/LLVM/bin/clang-cl.exe"
                    ],
                   cwd=sdl_dir,
                   )
    subprocess.run([CMD_FOR_VS, "&", "cmake", "--build", "build"], cwd=sdl_dir)


def clone_and_build_shadercross():
    shadercross_path = os.path.join(WORKING_DIR, "SDL_shadercross")
    shadercross_dir = shadercross_path.replace("\\", "/")
    if not os.path.exists(shadercross_path):
        subprocess.run(
            [
                "git",
                "clone",
                "--recursive",
                "https://github.com/libsdl-org/SDL_shadercross",
                shadercross_path,
            ],
        )

    subprocess.run(["git", "fetch", "origin"], cwd=shadercross_path)

    subprocess.run(
        ["git", "reset", "--hard", "--recurse-submodules", "origin/main"],
        cwd=shadercross_path,
    )

    spirv_headers_path = os.path.join(shadercross_path, "external", "SPIRV-Headers")
    spirv_headers_dir = spirv_headers_path.replace("\\", "/")
    # build_project(spirv_headers_path, [], "Release")
    #
    spirv_tools_path = os.path.join(shadercross_path, "external", "SPIRV-Tools")
    spirv_tools_dir = spirv_tools_path.replace("\\", "/")
    # build_project(spirv_tools_path, [f"-DSPIRV-Headers_SOURCE_DIR={spirv_headers_dir}", ], "Release", True)
    #
    spirv_cross_path = os.path.join(shadercross_path, "external", "SPIRV-Cross")
    spirv_cross_dir = spirv_cross_path.replace("\\", "/")
    # build_project(
    #     spirv_cross_path,
    #     [
    #         f"-DSPIRV-Headers_SOURCE_DIR={spirv_headers_dir}",
    #         f"-DSPIRV-Tools_SOURCE_DIR={spirv_tools_dir}"
    #     ],
    #     "Release",
    #     True
    # )

    directx_path = os.path.join(shadercross_path, "external", "DirectXShaderCompiler")
    # build_project(
    #     directx_path,
    #     [
    #         f"-DDXC_EXTERNAL_ROOT_DIR={shadercross_dir}/external",
    #         "-C ./cmake/caches/PredefinedParams.cmake"
    #     ],
    #     "Release",
    #     True
    # )

    sdl_dir = os.path.join(WORKING_DIR, "SDL", "build").replace("\\", "/")
    build_project(
        shadercross_path,
        [
            "-DSDLSHADERCROSS_SPIRVCROSS_SHARED=ON",
            "-DSDLSHADERCROSS_SPIRVCROSS_SATIC=OFF",
            "-DSDLSHADERCROSS_VENDORED=ON",
            f"-DSDL3_DIR={sdl_dir}",
            # f"-DCMAKE_PREFIX_PATH='{sdl_dir}';'{spirv_cross_dir}/build'",
            # f"-DSDL3_DIR={shadercross_dir}../SDL/build",
            # f"-Dspirv-cross-c_DIR={spirv_cross_dir}/build",
        ],
        "Release",
        True
    )


def build_project(cwd, dargs, config="Debug", install=False):
    build_dir = os.path.join(cwd, "build")

    if os.path.exists(build_dir):
        shutil.rmtree(build_dir)

    os.makedirs(build_dir)

    subprocess.run(
        [
            CMD_FOR_VS,
            "&",
            "cmake",
            "-B build",
            "-S .",
            f"-DCMAKE_BUILD_TYPE={config}"
        ] + dargs,
        cwd=cwd,
    )

    if install:
        subprocess.run([CMD_FOR_VS, "&", "cmake", "--build", "build",], cwd=cwd)


def install_binaries_windows():
    shadercross_build_path = os.path.join(WORKING_DIR, "SDL_shadercross", "build")
    spirv_cross_build_path = os.path.join(shadercross_build_path, "external", "SPIRV-Cross", "build")
    directx_build_path = os.path.join(shadercross_build_path, "external", "DirectXShaderCompiler", "bin")
    sdl_build_path = os.path.join(WORKING_DIR, "SDL", "build")

    shutil.copy(os.path.join(shadercross_build_path, "shadercross.exe"), BIN_INSTALL_DIR)
    shutil.copy(os.path.join(spirv_cross_build_path, "spirv-cross-c-shared.dll"), LIB_INSTALL_DIR)
    shutil.copy(os.path.join(directx_build_path, "dxcompiler.dll"), LIB_INSTALL_DIR)
    shutil.copy(os.path.join(sdl_build_path, "SDL3.dll"), LIB_INSTALL_DIR)


def cleanup():
    shutil.rmtree(os.path.join(WORKING_DIR, "SDL", "build"))
    shutil.rmtree(os.path.join(WORKING_DIR, "SDL_shadercross", "build"))


if __name__=="__main__":
    main()