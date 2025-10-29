# This is the main build file for the entire Brick-Link project.
cmake_minimum_required(VERSION 3.10)

# Tell the Visual Studio generator to build for the "Win32" (x86) platform.
# This ensures our DLL and executables are 32-bit, matching the game.
# (Moved BEFORE project() to ensure it applies globally.)
if(MSVC)
    set(CMAKE_GENERATOR_PLATFORM Win32)
endif()

project(BrickLink)

# Tell CMake where to put the final executables and DLLs for convenience.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Tell CMake to find and process the build instructions for our sub-projects.
add_subdirectory(Injector)
add_subdirectory(Patcher)
add_subdirectory(ModManager)