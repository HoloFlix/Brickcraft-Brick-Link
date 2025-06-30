# Brickcraft Modding & Development Guide

This document summarizes all known discoveries, techniques, and challenges related to modding the **Brickcraft prototype (circa 2012)** using Lua scripting and reverse engineering.

---

## 1. Introduction to Brickcraft Modding

Brickcraft is a voxel-based prototype that features multiplayer networking, Lua scripting, and a building system. Modding efforts revolve around extending gameplay, adding tools, and introducing quality-of-life features. Lua scripting is performed on the server side, while UI elements are built with Adobe Flash (SWF), making graphical modding a substantial challenge.

---

## 2. Core Modding Principles

### 2.1 Lua Scripting

- Lua logic executes server-side.
- Lua interacts with C++ objects exposed via `luabind`.
- Mod files like `Main.lua` and custom scripts such as `MyFirstMod.lua` are loaded during startup.
- The scripting environment supports object-oriented Lua using a class helper (`class 'Child'(Parent)` syntax).
- No global `Update(dt)` or `Tick()` exists; per-player logic must hook into `Item:Update(dt, player)`.

### 2.2 Reverse Engineering Tools

- **Ghidra**, **dotPeek**, **Hex editors**: Used to explore and map class bindings and server logic.
- Focus is on analyzing `Rex-Kwon-Do Server.exe` and `luabind.dll` for function exposure.

---

## 3. Discovered Lua API (Confirmed, Partially Working, and Problematic)

A near-complete map of the Lua bindings has been compiled. Some functions are confirmed in the binary but not callable at runtime.

| Object/Class     | Method/Property           | Status                  | Notes |
|------------------|---------------------------|--------------------------|-------|
| **Player**       | `:PushPlayer(vector)`     | ✅ Confirmed Working      | Core of fly command. |
|                  | `:SetItem(item)`          | ✅ Confirmed Working      | Expects `RKD::ItemWrapper`. |
|                  | `:GetGuidAsInteger()`     | ✅ Confirmed Working      | Returns player ID. |
|                  | `:GetName()`              | ✅ Confirmed Working      | Returns player name. |
|                  | `:GetViewDirection()`     | ✅ Confirmed Working      | Direction vector. |
|                  | `:GetHealth()`            | 🟧 In Binary              | Untested. |
|                  | `:SetHealth(number)`      | 🟧 In Binary              | Untested. |
|                  | `:GetEyePosition()`       | 🟧 In Binary              | Untested. |
|                  | `:GetPosition()`          | ❌ Problematic            | Found in binary, fails in Lua. |
|                  | `:SetPosition(vector)`    | ❌ Problematic            | Ditto. |
| **EntityManager**| `.Players`                | ✅ Confirmed Working      | Iterable player list. |
| **Vector3**      | `Vector3(x, y, z)`        | ✅ Confirmed Working      | Vector construction. |
| **ServerInstance**| Various methods          | ✅ Confirmed Working      | Spawning entities, importing LXFML. |
| **ServerCmd**    | All functions             | ✅ Confirmed Working      | All custom commands. |
| **Item**         | `Item()`, `:Update()`     | 🟧 Partially Working      | Custom item subclasses error. |

---

## 4. Current Limitations & Challenges

- **Position Functions Not Callable:** `GetPosition` and `SetPosition` are visible in the binary but not bound to Lua at runtime. Likely causes include casing issues, binding failures, or being part of a different class.
- **Custom Item Type Mismatch:** `SetItem(FlyItem)` fails with `nil value`, indicating our custom items aren't valid `ItemWrapper` objects. Current workaround using `Item()` constructor returns usable userdata, but inheritance fails.
- **No Global Tick Hook:** Logic must live in `Item:Update()` because no global update method is invoked.
- **UI Modding:** UI is Flash-based and controlled via native code. Modding it requires Flash/ActionScript reverse engineering and possibly custom native bindings.
- **World Editing:** World data lives in `world.db` as compressed binary blobs. Editing this format remains difficult.

---

## 5. Reverse Engineering Techniques & Findings

Key techniques that led to success:

- **String XREFs:** Searching for known method names (like `PushPlayer`) and following cross-references in Ghidra to locate `ServerCommander::RegisterPlayerVtable`.
- **Function Inspection:** Revealed class-method bindings (`Player`, `EntityManager`, `Vector3`, etc.).
- **Lua Engine Hooks:** Verified `luabind` runtime usage and absence of global update hooks via inspection of `lua_getglobal`, `lua_pcallk`, etc.

---

## 6. Implemented/Attempted Mods (with Status)

| Command          | Purpose                              | Status         | Notes |
|------------------|---------------------------------------|----------------|-------|
| `/coords`        | Display player coordinates            | ❌ Not working | `GetPosition()` unavailable. |
| `/teleport`      | Move player to X,Y,Z                  | ❌ Not working | `SetPosition()` unavailable. |
| `/fly`           | Apply upward force continuously       | ❌ Not working | `FlyItem` class fails `SetItem()`. |
| `/getname [guid]`| Return player name                    | ✅ Working      | Defaults to self if no GUID. |
| `/lookvec`       | Show direction vector                 | ✅ Working      | Uses `GetViewDirection`. |
| `/greetplayers`  | Print message to all players          | ✅ Working      | Console only. |
| `/givebrick`     | Add brick to inventory                | ✅ Working      | Server-side only. |
| `/listinventory` | Show inventory                        | ✅ Working      | Console view only. |
| `/chat <msg>`    | Console relay chat                    | ✅ Working      | Not a full UI chat interface. |

---

## 7. Future Modding Directions & Research

### 🔍 Fix the Position Methods

- **Hypothesis 1:** Case mismatch in function names (e.g., `getposition` vs `GetPosition`).
  - ✅ **Next Step:** Try lowercase/camelCase versions from Lua.
- **Hypothesis 2:** Functions live on a parent class (`BaseEntity`).
  - ✅ **Next Step:** Analyze `Player` inheritance in Ghidra. Check `luabind::class_<BaseEntity>` bindings.

### 🔍 Fix the Custom Item Type

- **Key Goal:** Create an `Item` subclass accepted by `SetItem`.
  - ✅ **Next Step:** Bypass inheritance temporarily. Call `Item()` directly and attach methods manually.
  - Investigate how `luabind` constructs `ItemWrapper` objects.

### 🔍 Implement Untested Functions

- **Test functions:** `GetHealth`, `SetHealth`, `GetEyePosition`.
  - ✅ **Next Step:** Build debug commands to inspect their return values and input formats.

### 🔍 Explore Client-Side Connectivity

- Check for `--ip`, `--connect` arguments in `Rex-Kwon-Do.exe` for easier server joins.
- Reverse engineer how config files or input flows work client-side.

### 🔍 World Editing Potential

- Investigate APIs like `SetBlock(x,y,z,type,color)` or reverse engineer `world.db` blob data format for custom building tools.

### 🔍 Event Hooks

- Investigate if events like `OnPlayerHit`, `OnBlockDestroy`, or `OnMessageReceived` exist and are exposed to Lua.

---

## Conclusion

Brickcraft modding has advanced from basic exploration to a deep understanding of its Lua-C++ bindings. The path forward involves solving two key technical barriers—position control and item creation—and building upon the nearly complete API map. This guide will continue evolving as further breakthroughs occur.

**Happy hacking!**
