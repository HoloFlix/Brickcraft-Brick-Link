# Brick-Link: ImGui Debug Console for Brickcraft

**Brick-Link** is an open-source proof-of-concept modding toolkit for *Brickcraft* (a leaked prototype of a LEGO-themed Minecraft sandbox game, internally codenamed "Rex-Kwon-Do"). It provides a DLL injector, runtime patcher, and a user-friendly Mod Manager GUI to inject an ImGui-based debug console into the game client and server. 

The console allows for basic command execution over the network (client-to-server), with hooks for logging decompressed world data. **This is an early prototype—features like full console functionality are a work-in-progress (WIP). Use at your own risk!**

## Features
- **Mod Manager GUI**: A standalone SDL2/OpenGL app with ImGui for easy configuration, file browsing, and one-click launching/injection of the client and server.
- **DLL Injector**: Command-line tool (`Injector.exe`) for manual DLL injection into running processes.
- **Patcher DLL**: Core runtime hooks using MinHook for:
  - ImGui overlay console (toggle with F2).
  - Network command forwarding (client sends commands to server via RakNet).
  - Optional logging of decompressed world data (client/server).
  - Mouse mode toggling for seamless gameplay.
- **Server-Side Hooks**: Intercepts incoming packets to execute commands in the game engine.
- **Configurable Logging**: Thread-safe debug logs to `brick-link-client.log` or `brick-link-server.log`.
- **Cross-Process Support**: Handles both client (`Rex-Kwon-Do.exe`) and server (`Rex-Kwon-Do Server.exe`).

## Requirements
- **OS**: Windows 10/11 (32-bit target for compatibility with the game).
- **Game Files**: Legally obtained *Brickcraft* prototype (`Rex-Kwon-Do.exe` and `Rex-Kwon-Do Server.exe`).
- **Build Tools** (if compiling):
  - CMake 3.10+.
  - Visual Studio 2019+ (with C++ workload) or MinGW.
- **Runtime Dependencies** (included in releases):
  - Vendor libs: MinHook, ImGui, SDL2, GLEW.
  - Game DLLs: `RakNet.dll`, `SDL.dll` (assumed present in game dir).

No internet required—everything is offline.

## Quick Start
1. **Download**: Grab the latest release from [Releases](https://github.com/yourusername/Brick-Link/releases).
2. **Extract**: Unzip to a folder (e.g., `C:\Brick-Link`).
3. **Configure Paths**:
   - Run `ModManager.exe`.
   - Set paths to your `Rex-Kwon-Do.exe`, `Rex-Kwon-Do Server.exe`, and the included `Patcher.dll`.
   - Enable logging options if desired (saves to `patch_config.ini`).
4. **Launch**:
   - Click **Launch & Patch BOTH** to start the server, inject the patcher, then launch/inject the client.
   - In-game: Press **F2** to toggle the ImGui console.
5. **Test Commands**: Type a command (e.g., game-specific like `/give brick`) and hit Enter or "Run". It sends via RakNet for server execution.

For manual injection: Run `Injector.exe "Rex-Kwon-Do.exe"` (after starting the process).

## Building from Source
The project uses CMake for cross-platform builds. Vendor libraries (MinHook, ImGui, SDL2, GLEW) are included in `/vendor`.

### Steps
1. **Clone Repo**:
   ```
   git clone https://github.com/yourusername/Brickcraft-Brick-Link.git
   cd Brick-Link
   ```

2. **Generate Build Files**:
   - Open a terminal in the root dir.
   - Run:
     ```
     mkdir build && cd build
     cmake .. -A Win32  # Forces 32-bit for game compatibility
     ```

3. **Build**:
   - Visual Studio: Open `Brick-Link.sln` in VS and build (Release/x86).
   - Command-line:
     ```
     cmake --build . --config Release
     ```

4. **Output**: Binaries in `build/bin/` (e.g., `Injector.exe`, `Patcher.dll`, `ModManager.exe`).
   - DLLs like `SDL2.dll` are auto-copied by CMake.

**Notes**:
- Ensure `/vendor` is intact—do not delete.
- For custom vendors: Update paths in subproject `CMakeLists.txt` files.

## Usage
### Mod Manager GUI
- **Paths Section**: Browse for executables and `Patcher.dll`.
- **Patch Options**: Toggle logging for world data decompression (useful for modding analysis).
- **Launch Button**: Starts server first (with 500ms delay), then client. Injection happens automatically in suspended mode for reliability.

### Injector CLI
```
Injector.exe "Rex-Kwon-Do.exe"
```
- Finds PID by process name.
- Injects `Patcher.dll` (must be in same dir).
- Run as Admin if access denied.

### In-Game Console
- **Toggle**: F2 (hides/shows overlay; restores mouse control).
- **Commands**: Enter text and Run/Enter. Sent as `ID_PLAYER_ACTION_PACKET` (136) via RakNet.
- **Status**: Shows "Connected" when RakPeer is captured.

### Logging
- Client: `brick-link-client.log` (in game dir).
- Server: `brick-link-server.log`.
- Use DebugView (Sysinternals) for real-time `OutputDebugString` output.

## Known Issues & Limitations
- **Console WIP**: Command parsing/execution is basic—only forwards strings to server `RunLine`. No validation or feedback yet.
- **Compatibility**: Tested on Brickcraft (Jun 28, 2012 prototype).
- **RakNet Hacks**: Relies on mangled names (e.g., `?Write@BitStream@RakNet@@QAEXQBD@Z`). If exports change, rebuild with pattern scanning.
- **No Multiplayer Beyond Local**: Assumes loopback; remote servers untested.
- **Performance**: ImGui hooks add minor overhead—disable for production play.
- **32-Bit Only**: Matches game arch; no x64 support.

## Troubleshooting
- **Injection Fails**: Run as Admin. Ensure game is 32-bit and not anti-cheat protected.
- **No Overlay**: Check if `opengl32.dll` and `SDL.dll` load. Verify hooks in logs.
- **RakNet Errors**: Confirm `RakNet.dll` exports match (use Dependency Walker).
- **Build Errors**: Missing vendors? Re-clone. CMake warnings? Ignore non-critical.
- **Crashes**: Disable logging first. Use VS debugger on `Patcher.dll`.

---
