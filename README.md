# The Brickcraft Modding & Development ReadMe

This readme serves as the definitive source of truth for the Brickcraft prototype, detailing the full extent and limitations of its modding capabilities.

## 1. Executive Summary

This project has successfully reverse-engineered the Lua scripting API for the 2012 Brickcraft prototype. Through a combination of binary analysis (using IDA and Ghidra) and live testing, we have defined the precise boundaries of what is possible with server-side Lua mods.

**Conclusion:** While a fun and surprisingly powerful set of physics-based mods can be created, core features required for more advanced gameplay—such as direct player position control and client-side inventory updates—are not exposed to Lua. These functions exist in the C++ engine but were never bound for script access.

Further significant progress in Brickcraft modding will require moving beyond pure Lua and developing external tools to patch the game's executable, or the ambitious goal of developing a new, custom client.

## 2. The Final Lua Mod Script

The culmination of this research is a stable, polished, and fully functional mod script (`MyFirstMod.lua`) that pushes the Lua API to its absolute limits.

**Key Features:**

- **Working Jetpack:** Implements a `/fly` command that equips the game's built-in Jetpack and uses a technique called "monkey patching" to overwrite the item's Update function in memory, allowing for a toggleable hover effect.
- **Physics Tools:** Includes a `/launcher` command for a vertical hop and a `/shockwave` command that pushes all players upwards, demonstrating the power of the `PushPlayer` function.
- **Administrative Commands:** Includes working commands like `/getname` to identify players.
- **Self-Documenting Limits:** Commands that were found to be impossible (like `/coords` and `/teleport`) have been disabled with informational messages to guide future modders.

## 3. The Lua API: A Complete Reference

| Object/Class    | Method/Property              | Status    | Description |
|----------------|------------------------------|-----------|-------------|
| Player         | `:PushPlayer(vector)`        | ✅ WORKING | Applies a physical force to the player object. |
|                | `:SetItem(item)`             | ✅ WORKING | Equips a native item (like the global JetPack object). |
|                | `:GetGuidAsInteger()`        | ✅ WORKING | Returns the player's unique 64-bit numerical ID. |
|                | `:GetName()`                 | ✅ WORKING | Returns the player's name as a string. |
| ServerCmd      | (Various)                    | ✅ WORKING | The entire command system (`AddCommand`, `AddAlias`, etc.) is functional. |
| EntityManager  | `.Players`                   | ✅ WORKING | An iterable collection of all currently connected players. |
| Vector3        | `Vector3(x,y,z)` & math ops  | ✅ WORKING | The 3D vector class is fully functional. |
| JetPack        | `JetPack()` & `:Update()`    | ✅ WORKING | The game's native Jetpack class can be instantiated and its Update method can be overwritten. |

## 4. Confirmed API Limitations (The "Glass Ceiling")

Our research has definitively proven that the following functions, despite existing in the C++ engine, were never bound to Lua and are therefore impossible to call from scripts.

| Inaccessible Function  | C++ Location         | Impact of Limitation |
|------------------------|----------------------|-----------------------|
| `GetPosition()`        | `RKD::BaseEntity`    | Cannot read player coordinates. Disables `/coords`. |
| `SetPosition()`        | `RKD::BaseEntity`    | Cannot set player coordinates. Disables `/teleport`. |
| `GetViewDirection()`   | `RKD::Player`        | Cannot get the player's look vector. Disables targeted tools. |
| `GetEyePosition()`     | `RKD::Player`        | Cannot get the player's camera position. Disables raycasting. |
| `AdjustNumBlocks()`    | `RKD::Inventory`     | Cannot modify the native inventory. Disables hotbar updates. |
| Custom Item Creation   | N/A                  | Cannot create new native item types from Lua tables. |

## 5. The Path Forward: Advanced Modding

The future of Brickcraft modding lies in C++-level modifications.

- **The Patcher/Injector Tool:** This is the most viable and powerful next step. Such a tool would load `Rex-Kwon-Do Server.exe` and inject code to add the missing Lua bindings for `GetPosition`, `AdjustNumBlocks`, etc., effectively removing the limitations detailed above and empowering Lua modders to create truly advanced features.
