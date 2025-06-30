# The Brickcraft Modding & Development Guide (Definitive Edition)

This final guide is a comprehensive document detailing our complete understanding of the prototype's capabilities and limitations.

## 1. Executive Summary

This project has successfully reverse-engineered the Lua scripting API for the 2012 Brickcraft prototype.

**Conclusion:** While a fun and stable set of physics-based mods are possible, core features like player position control and client-side inventory updates are not accessible from Lua. The functions for these features exist internally but were never exposed to the scripting engine. Further progress will require C++-level modifications via external tools.

## 2. Core Modding Principles: The Three Rules

Our research has revealed three unbreakable rules for modding this game:

- **The API is Fixed:** The callable functions we have discovered are the only functions available. The game engine does not expose its entire internal C++ API to Lua.
- **State Must Be Managed Globally:** Custom player states (like flight status) must be stored in a separate, global Lua table keyed by the player's GUID. The native player userdata object cannot be reliably modified with new properties.
- **The Server is the Source of Truth:** All gameplay logic is server-side. Mods can only react to commands and apply physics; they cannot directly read client-side input or update the UI.

## 3. The Definitive Lua API

The following is a complete list of all working custom functions confirmed to be usable by modders.

| Object/Class    | Method/Property         | Status    |
|----------------|-------------------------|-----------|
| Player         | `:PushPlayer(vector)`   | ✅ WORKING |
|                | `:SetItem(item)`        | ✅ WORKING |
|                | `:GetGuidAsInteger()`   | ✅ WORKING |
|                | `:GetName()`            | ✅ WORKING |
| ServerCmd      | (All functions)         | ✅ WORKING |
| EntityManager  | `.Players`              | ✅ WORKING |
| Vector3        | (All functions)         | ✅ WORKING |
| JetPack        | `JetPack()`             | ✅ WORKING |

## 4. Confirmed Inaccessible Features (The Hard Limits)

The following internal C++ engine features are not accessible from Lua, and mods requiring them are impossible without binary patching.

- **Player Position:** (`GetPosition`, `SetPosition`)
- **Player View Direction:** (`GetViewDirection`, `GetEyePosition`)
- **Native Inventory System:** (`AdjustNumBlocks`, etc.)
- **True Custom Item Creation**

## 5. Final, Stable Mod Showcase

The final version of our mod script (`MyFirstMod.lua`) demonstrates the full potential of the available API:

- `/fly`: A reliable, toggleable flight command that equips/unequips the native JetPack item and correctly tracks player state.
- `/launcher`: A safe, physics-based "hop" ability.
- `/shockwave`: A server-wide physics pulse that gently pushes all players.

## 6. The Future: A C++ Horizon

The journey of Lua modding for Brickcraft has reached its conclusion. The next great challenge for the community is to venture into C++-level modifications. The path forward includes:

- **Documenting the Client:** Analyzing the client executable (`Rex-Kwon-Do.exe`), specifically the Client Packet Handler function already located, to create a full network protocol map.
- **Developing an Injector/Patcher Tool:** Creating an external program to surgically add the missing Lua bindings for functions like `GetPosition` and `AdjustNumBlocks` to the server executable. This is the most viable path to unlocking the game's true potential.
- **Custom Client Development:** The ultimate, long-term goal for the community, which would rely on the network protocol map and a deep understanding of the client's rendering and input systems.
