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
  - The player model is a simple stack of four 1x1 bricks, with the top brick having a different color to represent the "head" or camera position.
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
