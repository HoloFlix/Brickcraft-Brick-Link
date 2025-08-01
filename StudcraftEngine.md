# Studcraft Engine:

## Executive Summary

Studcraft Engine is a clean-room re-implementation of the unreleased 2012 "Rex Kwon Do" / Brickcraft prototype. With a fully functional C++ server and client built from scratch and a fully reverse-engineered network protocol, the project lays the groundwork for a robust, open-source creative sandbox. Future goals focus on procedural world generation, rich player interactions, modding support, and technical scalability.

## Current State

The project has achieved core functionality:

- **Custom C++ server and client** communicate over a fully reverse-engineered RakNet 3 protocol.
- **Procedural world generation**, player object replication, and basic control (WASD + mouse-look) are implemented.
- **Reverse-engineering efforts** revealed limitations in the original prototype's Lua API and runtime patching, prompting the shift to full re-implementation.

Technologies used:

- **RakNet 3** for networking
- **Bullet Physics 2.78** for simulation
- **SDL**, **GLEW**, **OpenGL** for client rendering
- **Lua**, **SQLite**, **PugiXML**, **Boost** for scripting, caching, data import, and concurrency

## World Generation & Persistence

Current capabilities:

- Flat-world generation (e.g. 16x16 or 50x50 tile planes)
- Server-authoritative procedural generation
- Compressed chunk data sent to clients (169 bytes per chunk, zlib compressed)

Planned enhancements:

- **Biomes** (Castle, Space, Pirates, etc.)
- **Terrain height variation** and **natural structures** (trees, caves, rocks)
- **LXFML import** for pre-fab structures using `PugiXML`
- **Persistent world changes** saved via re-implemented `world.db` logic

## Building Mechanics

Key upcoming features:

- **Brick placing & breaking** synced via server
- **Expanded brick catalog** based on reverse-engineered definitions
- **Support for new bricks** using custom `LegoSubPart` definitions
- **Client-side raycasting** for placement preview
- **Server-side validation** to prevent cheating

Example Brick Table:

| ID | Name      | Studs | Pegs | Type            |
| -- | --------- | ----- | ---- | --------------- |
| 28 | 2x4 Brick | 8     | Yes  | TYPE\_BLOCK     |
| 29 | 2x2 Brick | 4     | Yes  | TYPE\_BLOCK     |
| 32 | 2x1 Slope | 2     | No   | TYPE\_TOPSLOPE2 |
| 33 | 2x2 Plate | 4     | Yes  | TYPE\_PLATE     |

## Player Experience

Implemented:

- **Basic movement and control**
- **Spawn at valid physics-aware position**

Planned:

- **Character customization** (colors, minifigures)
- **Enhanced physics** (jumping, falling, ground detection)

## Modding & Extensibility

Future support includes:

- **GUI-based Mod Manager** using Dear ImGui
- **Improved Lua/C++ Bridge** with robust bindings
- **Mod loader** for both Lua and C++ mods
- **Open-source contribution model** with clear guidelines

Example Lua Bindings:

| C++ Function  | Lua Name          | Description            |
| ------------- | ----------------- | ---------------------- |
| TryPlaceBrick | PlaceBrick        | Place a brick in-world |
| FillBlock     | BreakBrick        | Remove a brick         |
| SetPosition   | SetPlayerPosition | Move player entity     |
| Raycast       | PhysicsRaycast    | Perform world raycast  |

## Client Enhancements

In place:

- **GLSL shaders** with Phong lighting, color palette texture (LegoColors.bmp), edge highlights, DoF, FBOs

Planned:

- **Complete UI** (menus, HUD, inventory)
- **Graphics features** (VBO batching, instancing, dynamic lighting)
- **Settings and accessibility improvements**

## Development Roadmap

| Feature                     | Category    | Complexity | Impact      |
| --------------------------- | ----------- | ---------- | ----------- |
| Place/Break Bricks          | Core        | Medium     | High        |
| World Persistence           | Core        | Medium     | High        |
| Expanded Brick Catalog      | Content     | Medium     | High        |
| Biomes & Natural Structures | World       | Medium     | Medium-High |
| LXFML Import                | Integration | Medium     | High        |
| Mod Manager GUI             | Tools       | Medium     | High        |
| Lua/C++ Bridge Expansion    | Tools       | Medium     | High        |
| Client UI                   | UI/UX       | Medium     | High        |
| Full Graphics Capability    | Visual      | High       | Medium      |
| Player Customization        | UX          | Low        | Medium      |

## Strategic Outlook

With core systems in place, the project is entering a full game development phase. The immediate focus is on implementing player building, persistent world changes, and enriching the gameplay loop. Long-term, Studcraft aims to become a fully mod-friendly, visually polished, open-source sandbox platform inspired by LEGO aesthetics and creative freedom.




