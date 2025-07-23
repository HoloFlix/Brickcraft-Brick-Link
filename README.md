*This was summarized using notebook llm

# Brickcraft (2012 Prototype) 

## Overall Development Progress & Project Status

**Project**: Brick-Kwon-Do  
**Current Status**: Phase 2 - C++ Engine Re-implementation & Tooling  

### I. Project Summary
This project was initiated to explore and overcome the modding limitations of the 2012 Brickcraft prototype. Phase 1, which focused on Lua scripting and external C++ runtime patching, has been concluded. Extensive testing definitively proved that the prototype's engine architecture is fundamentally resistant to stable, complex runtime modification.

The project is now entering Phase 2, which shifts focus from patching the old engine to re-implementing it.

### II. Major Accomplishments (The Foundational Knowledge)
- **Complete Lua API Mapping**: The original server-side Lua API has been fully mapped, and its hard limitations (no access to position, native inventory, or view direction) have been definitively proven.
- **Discovery of the Core World Engine**: The `CityGenerator` and `LegoWorldGenerator` classes have been identified as the heart of world creation. Crucial functions like `TryPlaceBrick` and `FillBlock` have been located, providing the exact blueprints for block manipulation.
- **Identification of the Physics Engine**: The server uses the open-source Bullet Physics Library. This is a monumental discovery, as it means we can use existing public documentation to replicate the game's physics instead of reverse-engineering them from scratch.
- **Deconstruction of the Rendering Pipeline**: The client-side GLSL shaders have been acquired. Analysis reveals a sophisticated rendering engine for its time, featuring:
  - Phong lighting for realistic surface shading.
  - A 1D lookup texture for a global color palette.
  - Custom effects like brick edge highlighting.
  - An advanced Depth of Field (DoF) post-processing effect.
- **Mapping the Network Protocol**: The server and client executables use the RakNet library. Initial analysis of the client's packet handler has provided a foundational map of the network Packet IDs.

### III. Current Project Goal
The primary goal of Phase 2 is to begin development of a new, custom server application that the original game client can connect to.

---

## The Brickcraft Engine - A Teardown

This document consolidates our complete *current* understanding of the prototype's architecture.

### A. The Server
- **World Engine**: The master class is a `CityGenerator` singleton, which contains a `LegoWorldGenerator` instance. All block placement logic is handled by the native C++ function `RKD::LegoWorldGenerator::TryPlaceBrick(...)`, which is our primary target for re-implementation.
- **Player Data**:
  - The player model is a simple stack of four 2x2 bricks, with the top brick having a different color to represent the "head" or camera position.
  - Player inventory is managed by a native C++ `Inventory` class, but the functions to modify it (`AdjustNumBlocks`) were never exposed to Lua, preventing client-side hotbar updates from scripts.
- **Scripting**: Uses a standard Lua 5.1 environment with a limited, fixed API bound via `luabind.dll`.

### B. The Client
- **Rendering**: The client is a 32-bit OpenGL application using SDL for windowing. All visuals are controlled by a surprisingly advanced set of GLSL shaders that handle lighting, fog, color, and post-processing effects like Depth of Field. The game's distinct aesthetic can be fully replicated by studying these shader files.
- **UI**: The user interface is rendered by Autodesk's Scaleform GFx middleware, which interprets `.swf` (Adobe Flash) files. Reverse-engineering revealed that the UI has minimal scripting; its logic is almost certainly driven by C++ code within the client, making UI modifications without a new client nearly impossible.

---

# Brickcraft Lua Modding Guide (Definitive Edition)

This guide is for developers who wish to create Lua mods using the un-patched, original game server.

## I. The Unbreakable Rules of Lua Modding:

Based on our research, the un-patched game engine has four hardcoded limitations:

* **The API is Fixed:** The functions listed below are the *only* ones available.
* **Items Cannot Be Unequipped:** `player:SetItem(nil)` will crash the server. Flight via the `/fly` command is a permanent, one-way action for that session.
* **Many Native Behaviors are Unstoppable:** The default "hover" effect of the Jetpack is coded in C++ and cannot be turned off from Lua while the item is equipped.
* **The Physics Engine is Fragile:** Applying large forces via `PushPlayer` will cause players to clip through the terrain. All forces must be small and gentle.

## II. The Definitive Lua API:

* **Player:** `:PushPlayer(vector)`, `:SetItem(item)`, `:GetGuidAsInteger()`, `:GetName()`
* **ServerCommander:** All functions for creating commands are stable.
* **EntityManager:** The `.Players` iterator is stable.
* **JetPack:** The `JetPack()` constructor returns a working item.

## III. Feasible "Vanilla" Lua Mods:

* **Physics-Based Tools:** A `/hop` command with a very small upward force, or a `/nudge` command that applies a tiny force to another player.
* **Server-Side Economy:** A virtual economy can be tracked entirely in Lua tables, with commands like `/givecredits <player> <amount>`. This will have no in-game visual representation.
* **The One-Way Jetpack:** A stable command that permanently equips the Jetpack.


* **Engine Architecture:** The world is orchestrated by a master CityGenerator class, which owns and manages a LegoWorldGenerator instance. This LegoWorldGenerator is the workhorse responsible for the core block manipulation functions (TryPlaceBrick, FillBlock).
* **Data Storage:** The world is stored in a SQLite database (world.db) as a key-value store. The mapdata table contains chunk data, where the key is the chunk's coordinate string ("X,Y,Z") and the value is a binary BLOB.
* **Data Compression (The Great Cipher):** The binary BLOBs are not standard ZLIB streams. They are processed by a proprietary StreamCompressor class which reads a custom 4-byte header (containing the uncompressed size) before passing the rest of the payload to the ZLIB uncompress function. This was your most critical discovery.
* **Rendering:** The client uses OpenGL, managed by SDL for windowing. The visual style is defined by a set of GLSL shaders that implement Phong lighting, fog, and advanced effects like a 1D texture-based color palette and Depth of Field post-processing.
* **Networking:** All communication is handled by the RakNet library. We have already reverse-engineered the Packet IDs for the four most basic player actions (0x83 for Break, 0x85 for Color, 0x88 for Place/Select).
* **Physics:** The server uses the open-source Bullet Physics Library, a known and well-documented standard. This is a massive advantage.
* **Scripting:** The game uses luabind to expose a limited set of C++ functions to a Lua 5.1 scripting engine. We have proven that this API is fixed and cannot be extended without modifying the C++ executable.

---

# Brickcraft Patcher/Injector Tool: Research Insights and Strategic Pivot

For other developers and reverse engineers looking to create their own patcher/injector tool for the original Brickcraft client and server, our extensive research has provided clear insights into what has been attempted, the challenges encountered, and ultimately, what did not work, leading us to a strategic pivot.

## Initial Goals and Early Successes
Our project initially aimed to mod the unreleased 2012 Brickcraft prototype by extending its capabilities through its native Lua scripting API and later, through C++ runtime patching and injection. We successfully developed a functional C++ injector (BrickcraftInjector.exe) and a patcher DLL (BrickcraftPatcher.dll). This tooling was proven to successfully hook into the live server process without causing instability in a basic sense. A Mod Manager GUI prototype (BrickcraftModManager) was even created, demonstrating a working interface to launch the server and trigger the injection process.

## Why Patching the Original Engine Proved Unviable: A Comprehensive Breakdown
Despite these initial successes and significant effort, the approach of patching the original engine was definitively concluded to be a "dead end" for stable, complex modding. Here's a detailed explanation of why this path was abandoned:

## Lua API Limitations
The game's native Lua API was found to be highly restrictive. Crucial functionalities like direct player position control, native inventory management, and view direction were not exposed to Lua. This meant that even if a mod could inject Lua code, it couldn't achieve "Minecraft-level mods" without deeper engine access. Basic Lua modding was insufficient to manipulate core gameplay mechanics or create truly new features like custom classes without complex debugging.

## Engine Instability and Fragility
The original C++ game engine proved to be extremely fragile and fundamentally hostile to runtime modification. Attempts to inject code or place hooks often resulted in server crashes or silent bypasses. This indicated fundamental incompatibilities with modern patching techniques.

### Persistent Technical Roadblocks during Injection
* **Silent Crashes Post-Injection:** We frequently encountered scenarios where the injector reported success, but the server would immediately crash silently upon DLL loading, often before any custom MessageBox or log messages could be displayed. This pointed to fundamental incompatibilities between our modern C++ compiler/toolchain and the game's old, fragile C++ code, particularly concerning function hooking.

* **CREATE_SUSPENDED Flag Issues:** Our initial strategy involved launching the target process (e.g., Rex-Kwon-Do Server.exe) in a suspended state (CREATE_SUSPENDED) before injecting the DLL. However, it was discovered that the Rex-Kwon-Do Server.exe often acts as a launcher or stub that immediately spawns the client process when started in this suspended state. This meant injecting into the suspended server was often futile, as the client would appear, but the server logic wouldn't fully initialize or the server process would immediately close.

* **Inconsistent Injection Targets:** There was confusion about whether to inject into the server or the client. It was definitively confirmed that the server (Rex-Kwon-Do Server.exe) must be the target for patching, as it drives the game's dynamic world and logic. Injecting into the client directly was deemed pointless without a connected server.

* **Race Conditions in Hooking:** Even when injection seemed to work, we ran into race conditions where our hooks were placed after the target function had already executed during the server's startup sequence. This meant our custom code was never called, as the original function had already run, effectively bypassing our patch. Solutions attempting to inject into an already-running server were also complex and faced similar timing issues.

* **RakNet API Mismatches / Outdated Functions:** When attempting to register new Lua functions by hooking luabind.dll, we found that some helper functions we expected to exist (like QueryConstruction_PeerToPeer or AutoManage on ReplicaManager3) did not exist in the period-correct RakNet library version we were using. This required manual re-implementation of core logic that RakNet usually abstracts.

### Compiler and Linker Conflicts
* **Winsock War:** Frequent redefinition errors occurred due to conflicts between winsock.h (included by windows.h) and RakNet's requirement for winsock2.h. A global project fix using #define _WINSOCKAPI_ was needed, but its application across all files was complex.

* **Circular Dependencies and Incomplete Types:** Architectural mistakes in header file design led to complex circular inclusion problems (e.g., Server.h includes World.h, World.h includes Player.h, and Player.h tries to include Server.h), causing the compiler to fail. This required a major restructuring, including the creation of a DataTypes.h file to consolidate common struct definitions.

## Limited Scope for Future Expansion
Even if runtime patching were made stable, it would be inherently difficult to add entirely new features not present in the original prototype's architecture, such as completely new brick types with custom geometry or complex physics interactions, as modifications would be tied to a specific executable version.

## Original Client's Own Instability
Beyond server-side patching difficulties, the original Rex-Kwon-Do.exe client itself was found to be "buggy, unstable, and incomplete". Despite our new server's "perfect adherence to the reverse-engineered protocol," the client often got stuck in loops, froze, or sent duplicate packets, suggesting internal flaws that even a perfectly replicated server could not overcome. For example, it would sometimes send the 0x82 spawn confirmation packet twice, or stall out expecting a server heartbeat (0x29) that our server wasn't yet programmed to send (or was sending, but the client still crashed).

## Conclusion: The Strategic Pivot
These accumulating challenges led to the crucial decision to pivot away from patching the original prototype and instead, begin building a new, custom server and client from scratch. This new phase, known as the "Studcraft" or "The Brickcraft Restoration Project," leverages the vast body of reverse-engineered knowledge gained during the patching attempts.

The core insight was that debugging a broken original program through patching was less productive than using our comprehensive understanding of its architecture (including its network protocol, physics engine, and world generation) to build a stable, modern re-implementation. Our StudcraftServer and StudcraftClient are the results of this shift, offering a clean, open-source foundation for future development.
