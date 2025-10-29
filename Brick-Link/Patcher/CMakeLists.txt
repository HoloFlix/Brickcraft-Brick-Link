# This file builds the Patcher.dll payload.
project(Patcher)

# --- Define path to vendor libraries for clarity ---
set(VENDOR_DIR "${CMAKE_SOURCE_DIR}/vendor") # CMAKE_SOURCE_DIR is the root "Brick-Link" folder
set(IMGUI_DIR "${VENDOR_DIR}/imgui")

# --- 1. Build a MinHook library from its source files ---
# (This part remains the same)
file(GLOB MINHOOK_SOURCES
    "${VENDOR_DIR}/minhook/src/*.c"
    "${VENDOR_DIR}/minhook/src/hde/*.c"
)
add_library(MinHookLib STATIC ${MINHOOK_SOURCES})
target_include_directories(MinHookLib PUBLIC "${VENDOR_DIR}/minhook/include")


# --- 2. Build ImGui as a helper library FROM THE VENDOR FOLDER ---
add_library(ImGuiLib STATIC
    "${IMGUI_DIR}/imgui.cpp"
    "${IMGUI_DIR}/imgui_draw.cpp"
    "${IMGUI_DIR}/imgui_tables.cpp"
    "${IMGUI_DIR}/imgui_widgets.cpp"
    "${IMGUI_DIR}/imgui_impl_win32.cpp"
    "${IMGUI_DIR}/imgui_impl_opengl3.cpp"
)
# This is crucial: it tells other libraries that depend on ImGuiLib where to find imgui.h
target_include_directories(ImGuiLib PUBLIC ${IMGUI_DIR})


# --- 3. Build our Patcher.dll ---
add_library(Patcher SHARED src/dllmain.cpp)

# Tell the Patcher project where to find its headers (MinHook's and now ImGui's)
target_include_directories(Patcher PRIVATE
    "${VENDOR_DIR}/minhook/include"
    "${IMGUI_DIR}" # <- This lets you use #include "imgui.h"
)

# Link Patcher.dll against MinHook, ImGui, and the necessary system libraries
target_link_libraries(Patcher PRIVATE
    MinHookLib
    ImGuiLib
    opengl32 # For wglSwapBuffers and OpenGL calls
    gdi32    # For HDC and other windowing functions
)