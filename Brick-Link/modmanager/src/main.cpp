#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <commdlg.h>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include "SDL.h"
#include "GL/glew.h"
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"

// --- Global App State & Helpers ---

struct AppConfig {
    char serverPath[MAX_PATH] = "";
    char clientPath[MAX_PATH] = "";
    char patcherPath[MAX_PATH] = "";
    bool bLogClientDecompress = true;
    bool bLogServerDecompress = false;
};

// Forward declarations
std::string OpenFileDialog(const char* filter);
void LoadConfig(AppConfig& config);
void SaveConfig(const AppConfig& config);
void LaunchProcess(const char* exePath, const AppConfig& config);
void LaunchAndInject(const char* exePath, const char* dllPath, const AppConfig& config);

int main(int, char**) {
    // --- Standard SDL/OpenGL/ImGui Setup ---
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("Brick-Link Mod Manager", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 600, 350, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    SDL_GLContext gl_context = SDL_GL_CreateContext(window);
    glewInit();
    
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
    ImGui_ImplOpenGL3_Init("#version 150");
    
    AppConfig config;
    LoadConfig(config);

    // --- Main Loop ---
    bool done = false;
    while (!done) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT) done = true;
        }

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();
        
        ImGui::SetNextWindowPos(ImVec2(0,0));
        ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
        if (ImGui::Begin("Main", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings)) {
            ImGui::Text("Brick-Link Mod Manager");
            ImGui::Separator();
            
            if (ImGui::CollapsingHeader("File Paths", ImGuiTreeNodeFlags_DefaultOpen)) {
                ImGui::PushItemWidth(-80);
                ImGui::InputText("Server Exe", config.serverPath, MAX_PATH); ImGui::SameLine();
                if (ImGui::Button("...##S")) { std::string p = OpenFileDialog("Executable Files (*.exe)\0*.exe\0"); if(!p.empty()) strcpy_s(config.serverPath, p.c_str()); }
                
                ImGui::InputText("Client Exe", config.clientPath, MAX_PATH); ImGui::SameLine();
                if (ImGui::Button("...##C")) { std::string p = OpenFileDialog("Executable Files (*.exe)\0*.exe\0"); if(!p.empty()) strcpy_s(config.clientPath, p.c_str()); }

                ImGui::InputText("Patcher Dll", config.patcherPath, MAX_PATH); ImGui::SameLine();
                if (ImGui::Button("...##P")) { std::string p = OpenFileDialog("DLL Files (*.dll)\0*.dll\0"); if(!p.empty()) strcpy_s(config.patcherPath, p.c_str()); }
                ImGui::PopItemWidth();
            }

            if (ImGui::CollapsingHeader("Patch Options", ImGuiTreeNodeFlags_DefaultOpen)) {
                ImGui::Checkbox("Log Client Decompression", &config.bLogClientDecompress);
                ImGui::Checkbox("Log Server Decompression", &config.bLogServerDecompress);
                ImGui::Separator();
                ImGui::TextWrapped("Enable these hooks to log decompressed world data from the client or server to a file.");
            }

            ImGui::Separator();
            ImGui::Dummy(ImVec2(0, 10));

            bool paths_valid = (strlen(config.serverPath) > 0 && strlen(config.clientPath) > 0 && strlen(config.patcherPath) > 0);
            if (!paths_valid) ImGui::BeginDisabled();
            
			if (ImGui::Button("Launch & Patch BOTH", ImVec2(-1, 40))) {
				// Now we call LaunchAndInject for BOTH the server and the client.
				LaunchAndInject(config.serverPath, config.patcherPath, config);
				Sleep(500); // Give the server a moment to start up and initialize its hooks.
				LaunchAndInject(config.clientPath, config.patcherPath, config);
			}
            
            if (!paths_valid) ImGui::EndDisabled();
        }
        ImGui::End();

        // --- Rendering & Cleanup ---
        glClearColor(0.1f, 0.1f, 0.12f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        SDL_GL_SwapWindow(window);
    }

    SaveConfig(config);
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

// --- Helper Implementations ---

void SaveConfig(const AppConfig& config) {
    WritePrivateProfileStringA("Paths", "ServerPath", config.serverPath, ".\\settings.ini");
    WritePrivateProfileStringA("Paths", "ClientPath", config.clientPath, ".\\settings.ini");
    WritePrivateProfileStringA("Paths", "PatcherPath", config.patcherPath, ".\\settings.ini");
    
    // Save the patch config to a separate file that the DLL will read
    WritePrivateProfileStringA("Hooks", "LogClientDecompress", config.bLogClientDecompress ? "1" : "0", ".\\patch_config.ini");
    WritePrivateProfileStringA("Hooks", "LogServerDecompress", config.bLogServerDecompress ? "1" : "0", ".\\patch_config.ini");
}

void LoadConfig(AppConfig& config) {
    GetPrivateProfileStringA("Paths", "ServerPath", "", config.serverPath, MAX_PATH, ".\\settings.ini");
    GetPrivateProfileStringA("Paths", "ClientPath", "", config.clientPath, MAX_PATH, ".\\settings.ini");
    GetPrivateProfileStringA("Paths", "PatcherPath", "", config.patcherPath, MAX_PATH, ".\\settings.ini");
}

std::string GetDirectoryFromPath(const char* path) {
    std::string exeDir = path;
    size_t last_slash = exeDir.find_last_of("\\/");
    if (std::string::npos != last_slash) {
        return exeDir.substr(0, last_slash);
    }
    return "";
}

void LaunchProcess(const char* exePath, const AppConfig& config) {
    SaveConfig(config); // Save settings before launching
    std::string workingDir = GetDirectoryFromPath(exePath);

    STARTUPINFOA si = { sizeof(si) };
    PROCESS_INFORMATION pi;
    if (CreateProcessA(exePath, NULL, NULL, NULL, FALSE, 0, NULL, workingDir.c_str(), &si, &pi)) {
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
        std::cout << "Launched process: " << exePath << std::endl;
    } else {
        std::cerr << "CreateProcess failed for " << exePath << ". Error: " << GetLastError() << std::endl;
    }
}

void LaunchAndInject(const char* exePath, const char* dllPath, const AppConfig& config) {
    SaveConfig(config); // Save settings before launching
    std::string workingDir = GetDirectoryFromPath(exePath);
    
    STARTUPINFOA si = { sizeof(si) };
    PROCESS_INFORMATION pi;
    if (!CreateProcessA(exePath, NULL, NULL, NULL, FALSE, CREATE_SUSPENDED, NULL, workingDir.c_str(), &si, &pi)) {
        std::cerr << "CreateProcess (for injection) failed: " << GetLastError() << std::endl;
        return;
    }
    
    char fullDllPath[MAX_PATH];
    GetFullPathNameA(dllPath, MAX_PATH, fullDllPath, NULL);
    void* loc = VirtualAllocEx(pi.hProcess, 0, sizeof(fullDllPath), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (loc && WriteProcessMemory(pi.hProcess, loc, fullDllPath, sizeof(fullDllPath), 0)) {
        HANDLE hThread = CreateRemoteThread(pi.hProcess, 0, 0, (LPTHREAD_START_ROUTINE)LoadLibraryA, loc, 0, 0);
        if (hThread) {
            WaitForSingleObject(hThread, INFINITE);
            CloseHandle(hThread);
            std::cout << "Successfully injected " << dllPath << " into " << exePath << std::endl;
        }
    } else {
         std::cerr << "Injection memory operation failed." << std::endl;
    }
    
    ResumeThread(pi.hThread);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}

std::string OpenFileDialog(const char* filter) {
    OPENFILENAMEA ofn;
    CHAR szFile[MAX_PATH] = { 0 };
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = filter;
    ofn.nFilterIndex = 1;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
    if (GetOpenFileNameA(&ofn)) return ofn.lpstrFile;
    return "";
}