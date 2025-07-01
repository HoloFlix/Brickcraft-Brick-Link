# Overall Development Progress & Project Status

**Project:** The Brickcraft Prototype (2012)
**Current Status:** Phase 2 - C++ Engine Patching & API Expansion

---

## I. Project Summary:

This project was initiated to explore the modding capabilities of the unreleased 2012 Brickcraft prototype. We have successfully moved beyond the initial limitations of the game's native Lua scripting API and have developed a **stable, working C++ injection and patching framework**.

---

## II. Major Accomplishments to Date:

* **Complete Lua API Mapping:** The entire server-side Lua API has been reverse-engineered, and its hard limitations have been definitively documented.
* **Discovery of Core Engine Components:** Critical C++ classes and functions have been located within the server executable, including:
    * The **CityGenerator** and **LegoWorldGenerator** classes, responsible for world creation.
    * The **TryPlaceBrick** function, the key to block manipulation.
    * The use of the **Bullet Physics Library**, providing a known standard for how physics are handled.
* **Development of a Stable C++ Injection Tool:** A functional C++ injector (**BrickcraftInjector.exe**) has been built and proven to work.
* **Development of a Stable C++ Patcher Stub:** A patcher DLL (**BrickcraftPatcher.dll**) has been developed that can successfully hook into the live server process **without causing instability**. It currently has an empty payload, awaiting final implementation.
* **Creation of a Mod Manager GUI Prototype:** A separate project (**BrickcraftModManager**) has been created, demonstrating a working GUI that can launch the server and trigger the injection process, laying the foundation for a user-friendly launcher.

---

## III. Current Project Goal:

The immediate and primary goal is to **fully implement the C++ patcher payload**. This involves solving the final hooking issue to enable our custom C++ functions to be called, which will in turn add the **GetPosition**, **SetPosition**, and **SetBlock** commands to the Lua API.

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
