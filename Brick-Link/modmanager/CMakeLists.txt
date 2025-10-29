project(ModManager)

# --- Define paths to our vendored libraries ---
set(VENDOR_DIR "${CMAKE_SOURCE_DIR}/vendor")
set(SDL2_INCLUDE_DIR "${VENDOR_DIR}/SDL2/include")
set(SDL2_LIBRARY_DIR "${VENDOR_DIR}/SDL2/lib/x86") # IMPORTANT: Using 32-bit libraries
set(GLEW_INCLUDE_DIR "${VENDOR_DIR}/glew/include")
set(IMGUI_DIR "${VENDOR_DIR}/imgui")

# --- Manually Define the Libraries ---

# 1. GLEW
add_library(GLEW STATIC "${VENDOR_DIR}/glew/src/glew.c")
target_include_directories(GLEW PUBLIC ${GLEW_INCLUDE_DIR})
# This define is required when building GLEW as a static library
target_compile_definitions(GLEW PRIVATE GLEW_STATIC)

# 2. ImGui
add_library(ImGui STATIC
    "${IMGUI_DIR}/imgui.cpp"
    "${IMGUI_DIR}/imgui_draw.cpp"
    "${IMGUI_DIR}/imgui_tables.cpp"
    "${IMGUI_DIR}/imgui_widgets.cpp"
    "${IMGUI_DIR}/imgui_impl_sdl2.cpp"  # Correct backend for SDL2
    "${IMGUI_DIR}/imgui_impl_opengl3.cpp"
)
target_include_directories(ImGui PUBLIC ${IMGUI_DIR} ${SDL2_INCLUDE_DIR})

# 3. SDL2
add_library(SDL2::SDL2 SHARED IMPORTED)
set_target_properties(SDL2::SDL2 PROPERTIES
    IMPORTED_LOCATION "${SDL2_LIBRARY_DIR}/SDL2.dll"
    IMPORTED_IMPLIB "${SDL2_LIBRARY_DIR}/SDL2.lib"
)
target_include_directories(SDL2::SDL2 INTERFACE ${SDL2_INCLUDE_DIR})

add_library(SDL2::main STATIC IMPORTED)
set_target_properties(SDL2::main PROPERTIES
    IMPORTED_LOCATION "${SDL2_LIBRARY_DIR}/SDL2main.lib"
)

# --- Create the ModManager executable ---
add_executable(ModManager src/main.cpp)

target_compile_definitions(ModManager PRIVATE GLEW_STATIC)

# --- Link Everything Together ---
target_include_directories(ModManager PRIVATE ${GLEW_INCLUDE_DIR} ${IMGUI_DIR} ${SDL2_INCLUDE_DIR})

target_link_libraries(ModManager PRIVATE 
    ImGui 
    GLEW 
    SDL2::SDL2
    SDL2::main
    opengl32 
)

# After build, copy the required DLLs to the executable's directory
add_custom_command(TARGET ModManager POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${SDL2_LIBRARY_DIR}/SDL2.dll"
    "$<TARGET_FILE_DIR:ModManager>"
)